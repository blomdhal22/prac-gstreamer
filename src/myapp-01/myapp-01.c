 #define LOG_TAG      "myapp-01"
 #define LOG_NDEBUG   0
 #include <logging.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <gst/gst.h>

#define GBOOLEAN_STR(x) x? "TRUE" : "FALSE"

static GstElement *pipeline;
static GstBus *bus;
static GMainLoop *loop;
static GstElement *bin;
static GstElement *tee;
static GstElement *filesink;

static gint eos_wait;

void add_bin()
{
	GstPad *src_pad, *sink_pad;

	g_print("Info: Adding filesink bin.\n");
	gst_bin_add(GST_BIN(pipeline), bin);
	if (!gst_element_sync_state_with_parent(bin))
	{
		g_printerr("Error: Unable to sync bin state with parent.\n");
		exit(1);
	}

	if(!(src_pad = gst_element_get_static_pad(tee, "src_1")))
	{
		g_printerr("Error: Failed to get the tee's src_pad [src_1].\n");
		exit(1);
	}

	if(!(sink_pad = gst_element_get_static_pad(bin, "sink")))
	{
		g_printerr("Error: Failed to get the bin's sink_pad [sink].\n");
		exit(1);
	}

	if (GST_PAD_LINK_FAILED(gst_pad_link(src_pad, sink_pad)))
	{
		g_printerr("Error: Failed to link the tee to the bin's sink_pad [sink].\n");
		exit(1);
	}

	gst_object_unref(src_pad);
	gst_object_unref(sink_pad);
}

GstPadProbeReturn
eos_remove_probe(GstPad *pad, GstPadProbeInfo *info, gpointer user_data)
{
	// g_print("Info: Received event on filesink.\n");
	if(GST_EVENT_TYPE (GST_PAD_PROBE_INFO_DATA (info)) != GST_EVENT_EOS)
		return GST_PAD_PROBE_PASS;

	g_print("Info: Received eos on filesink.\n");
	g_atomic_int_dec_and_test(&eos_wait);
	return GST_PAD_PROBE_REMOVE;
}

GstPadProbeReturn 
block_remove_probe(GstPad *pad, GstPadProbeInfo *info, gpointer user_data)
{
	GstPad *peer_pad, *sink_pad;
	GstElement *parent, *peer_parent;
	gchar *pad_name, *parent_name, 
		*peer_pad_name, *peer_pad_parent_name;

	pad_name = gst_pad_get_name(pad);
	parent = gst_pad_get_parent_element(pad);
	parent_name = gst_element_get_name(parent);

	g_print("Info: [%s:%s] is blocked.\n", parent_name, pad_name);

	if(!(peer_pad = gst_pad_get_peer(pad)))
	{
		g_printerr("Error: [%s:%s] pad is not linked.\n", parent_name, pad_name);
		goto block_remove_probe_exit;
	}
	peer_pad_name = gst_pad_get_name(peer_pad);
	peer_parent = gst_pad_get_parent_element(peer_pad);
	peer_pad_parent_name = gst_element_get_name(peer_parent);	
	g_print("Info: Sent eos on [%s:%s].\n", peer_pad_parent_name, peer_pad_name);	
	g_free(peer_pad_name);
	g_free(peer_pad_parent_name);
	gst_object_unref(peer_pad);
	gst_object_unref(peer_parent);


	sink_pad = gst_element_get_static_pad(filesink, "sink");
	gst_pad_add_probe(sink_pad, GST_PAD_PROBE_TYPE_EVENT_DOWNSTREAM, 
		eos_remove_probe, NULL, NULL);

	gst_pad_send_event(peer_pad, gst_event_new_eos());
	gst_object_unref(sink_pad);
	

block_remove_probe_exit:
	// g_print("Entered label\n"); 
	g_free(pad_name);
	g_free(parent_name);
	gst_object_unref(parent);
	return GST_PAD_PROBE_REMOVE;
}

void remove_bin()
{
	GstPad *src_pad;

	g_print("Info: Removing filesink bin.\n");

	if(!(src_pad = gst_element_get_static_pad(tee, "src_1")))
	{
		g_printerr("Error: Failed to get the tee's src_pad [src_1].\n");
		exit(1);
	}

	g_atomic_int_inc(&eos_wait);

	gst_pad_add_probe(src_pad, GST_PAD_PROBE_TYPE_BLOCK_DOWNSTREAM, 
		block_remove_probe, NULL, NULL);


	//lock until EOS
	while(eos_wait) usleep(10000);
	
	g_print("Info: Removing filesink bin.\n");
	if (gst_element_set_state(bin, GST_STATE_NULL) == GST_STATE_CHANGE_FAILURE)
	{
		g_printerr("Error: Failed to set bin state to NULL.\n");
		exit(1);
	}
	gst_object_ref(bin);
	gst_bin_remove(GST_BIN(pipeline), bin);

	gst_object_unref(src_pad);
}

gpointer
keyboard()
{
	gboolean recording = FALSE;

	while(TRUE)
	{
    getchar();
    recording = !recording;
    recording? add_bin() : remove_bin();
    g_print("Recording to /tmp/video.mp4 %s\n", GBOOLEAN_STR(recording));

    while(getchar() != '\n') ;
	}

	return NULL;
}

void 
tag_parser(const GstTagList *list, const gchar *tag, gpointer user_data)
{
	const gchar *type_name;
	GType type;
	GstMessage *message;

	message = (GstMessage*) user_data;
	type = gst_tag_get_type(tag);
	type_name = g_type_name(type);
	g_print("DEBUG: BUS: [%s] %s - %s=(%s)", GST_MESSAGE_SRC_NAME(message), GST_MESSAGE_TYPE_NAME(message), tag, type_name);

	if(!strcmp(type_name, "gchararray"))
	{
		gchar *value;
		gst_tag_list_get_string(list, tag, &value);
		g_print("%s\n", value);
		g_free(value);
	}
	else if(!strcmp(type_name, "guint"))
	{
		guint value;
		gst_tag_list_get_uint(list, tag, &value);
		g_print("%u\n", value);
	}

}

gboolean 
bus_message_parser_cb(GstBus *bus, GstMessage *message, gpointer data)
{
	char buffer[1024];
	buffer[0] = '\0';

	switch (GST_MESSAGE_TYPE(message))
	{
		case GST_MESSAGE_ERROR:
		{
			GError *err = NULL;
    		gchar *dbg_info = NULL;
    
    		gst_message_parse_error (message, &err, &dbg_info);
    		snprintf(buffer, 1024, "%s. %s", err->message, (dbg_info) ? dbg_info : "(NULL)");
		    g_error_free (err);
			g_free (dbg_info);
			g_main_loop_quit(loop);
		}
		case GST_MESSAGE_TAG:
		{
			GstTagList *tags = NULL;

		    gst_message_parse_tag (message, &tags);
		    gst_tag_list_foreach(tags, tag_parser, message);
		    gst_tag_list_unref (tags);
		    snprintf(buffer, 1024,"<end_of_tags>");
		    break;
		}
		case GST_MESSAGE_STATE_CHANGED:
		{
			GstState old_state, new_state;
			    
	    	gst_message_parse_state_changed (message, &old_state, &new_state, NULL);
	    	snprintf (buffer, 1024, "from %s to %s",
	        	gst_element_state_get_name (old_state), 
	        	gst_element_state_get_name (new_state));
			break;
		}
		case GST_MESSAGE_NEW_CLOCK:
		{
			GstClock *clock;
			
			gst_message_parse_new_clock(message, &clock);
			snprintf (buffer, 1024, "%s", GST_OBJECT_NAME(GST_OBJECT(clock)));
			break;
		}
		case GST_MESSAGE_STREAM_STATUS:
		{
			GstElement *owner;
			GstStreamStatusType type;
			gchar *name;
			char *type_str;

			gst_message_parse_stream_status(message, &type, &owner);
			name = gst_element_get_name(owner);
			switch (type)
			{
				case GST_STREAM_STATUS_TYPE_CREATE:
					type_str = "new thread needs to be created";
					break;
				case GST_STREAM_STATUS_TYPE_ENTER:
					type_str = "thread entered its loop function";
					break;
				case GST_STREAM_STATUS_TYPE_LEAVE:
					type_str = "thread left its loop function";
					break;
				case GST_STREAM_STATUS_TYPE_DESTROY:
					type_str = "thread is destroyed";
					break;
				case GST_STREAM_STATUS_TYPE_START:
					type_str = "thread is started";
					break;
				case GST_STREAM_STATUS_TYPE_PAUSE:
					type_str = "thread is paused";
					break;
				case GST_STREAM_STATUS_TYPE_STOP:
					type_str = "thread is stopped";
					break;
			}
			snprintf (buffer, 1024, "[%s]  %s", name, type_str);
			g_free(name);
			break;
		}
		case GST_MESSAGE_ELEMENT:
		{
			//reall depends on the element. So far ony xvimagesink is outputting but there's
			// nothing on the documentation about it
			break;
		}
		case GST_MESSAGE_LATENCY:
		{
			//nothing available to parse
			snprintf(buffer, 1024, "recalculate latency");
			break;
		}
		case GST_MESSAGE_ASYNC_DONE:
		{
			GstClockTime running_time = 0;

			gst_message_parse_async_done (message, &running_time);
			snprintf (buffer, 1024, "running_time=%lus", GST_TIME_AS_SECONDS(running_time));
			break;
		}
		case GST_MESSAGE_QOS:
		{
			gboolean live;
			guint64 running_time;
			guint64 stream_time;
			guint64 timestamp;
			guint64 duration;
			gint64 jitter;
			gdouble proportion;
			gint quality;
			GstFormat format;
			guint64 processed;
			guint64 dropped;
			char format_str[256];


			gst_message_parse_qos (message, &live, &running_time, &stream_time, &timestamp, &duration);
			gst_message_parse_qos_values (message, &jitter, &proportion, &quality);
			gst_message_parse_qos_stats (message, &format, &processed, &dropped);

			switch(format)
			{
				case GST_FORMAT_UNDEFINED:
				{
					format_str[0] = '\0';
					break;
				}
				case GST_FORMAT_DEFAULT:
				{
					snprintf(format_str, 256,"frames/samples_processed=%lu frames/samples_dropped=%lu", 
						processed, dropped);
					break;
				}
				case GST_FORMAT_BUFFERS:
				{
					snprintf(format_str, 256,"frames_processed=%lu frames_dropped=%lu", 
						processed, dropped);
					break;
				}
				case GST_FORMAT_TIME:
				{
					snprintf(format_str, 256,"ms_processed=%lu ms_dropped=%lu", 
						GST_TIME_AS_MSECONDS(processed), GST_TIME_AS_MSECONDS(dropped));
					break;
				}
				case GST_FORMAT_BYTES:
				{
					snprintf(format_str, 256,"bytes_processed=%lu bytes_dropped=%lu", 
						processed, dropped);
					break;
				}
				case GST_FORMAT_PERCENT:
				{
					snprintf(format_str, 256,"processed=%lu%% dropped=%lu%%", 
						processed, dropped);
					break;
				}
			}

			snprintf (buffer, 1024, "live=%s stream_time=%lums jitter=%ldus proportion=%.2f "
				"quality=%d%% ", GBOOLEAN_STR(live), GST_TIME_AS_MSECONDS(stream_time), 
				GST_TIME_AS_USECONDS(jitter), proportion, quality / 10000);

			strcat(buffer, format_str);

			break;
		}
		case GST_MESSAGE_STREAM_START:
		{
			guint group_id;

			if (gst_message_parse_group_id (message, &group_id))
				snprintf (buffer, 1024, "group_id=%u", group_id);
			else
				snprintf (buffer, 1024,  "group_id=(NULL)");

			break;
		}
		default:
			//not implemented yet.
			break;
	}

	printf("DEBUG: BUS: [%s] %s - %s\n", GST_MESSAGE_SRC_NAME(message), GST_MESSAGE_TYPE_NAME(message), buffer);	
	return TRUE;
}

int
main(int argc, char *argv[])
{

	GError *error = NULL;
  	guint bus_watch_id;
	gchar *pipeline_description, *bin_description;
	GThread *kb_thread;
	GstPad *pad;

	gst_init(&argc, &argv);
	loop = g_main_loop_new(NULL, FALSE);

	//quick assembly of the pipeline
	pipeline_description = "v4l2src device=/dev/video0 ! " 
		"image/jpeg, height=720 ! jpegdec ! tee name=tee "
		"tee. ! queue ! xvimagesink sync=false";
	bin_description = "queue ! x264enc tune=zerolatency ! mp4mux ! "
		"filesink name=filesink location=/tmp/video.mp4";

	pipeline = gst_parse_launch (pipeline_description, &error);
	if (!pipeline)
	{
		g_printerr ("Pipeline parse error: %s\n", error->message);
    	exit (1);
	}

	bin = gst_parse_bin_from_description(bin_description, TRUE, &error);
	if (!bin)
	{
		g_printerr ("Bin parse error: %s\n", error->message);
    	exit (1);
	}
	//verify that the pad is actually connected with the queue element
	//search here what is this proxy pad attached to 

	tee = gst_bin_get_by_name(GST_BIN(pipeline), "tee");
	// pad = gst_element_get_request_pad(tee, "src_%u");
  pad = gst_element_request_pad_simple(tee, "src_%u");
	if (!pad)
	{
		g_printerr ("Error: Failed to retrieve src_pad from tee\n");
    	exit (1);
	}

	filesink = gst_bin_get_by_name(GST_BIN(bin), "filesink");
	if (!filesink)
	{
		g_printerr ("Error: Failed to retrieve filesink from bin\n");
    	exit (1);
	}

	bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
	bus_watch_id = gst_bus_add_watch (bus, bus_message_parser_cb, loop);
	gst_object_unref (bus);

	gst_element_set_state (pipeline, GST_STATE_PLAYING);

	kb_thread = g_thread_new("keyboard", keyboard, NULL);

	g_main_loop_run(loop);

	
	gst_element_set_state(pipeline, GST_STATE_NULL);

	//free resources
	g_thread_unref(kb_thread);
	gst_object_unref (pipeline);
	gst_object_unref(bin);
	g_source_remove (bus_watch_id);
	g_main_loop_unref(loop);
	return 0;
}