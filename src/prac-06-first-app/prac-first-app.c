 #define LOG_TAG      "prac-06-first-app"
 #define LOG_NDEBUG   0
 #include <logging.h>

#include <gst/gst.h>
#include <glib.h>

static void seek_to_time(GstElement *pipeline, gint64 time_nanoseconds)
{
  if (!gst_element_seek(pipeline, 1.0, GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH,
                        GST_SEEK_TYPE_SET, time_nanoseconds,
                        GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE)) {
    DE("Seek failed!");
  }
}

static gboolean cb_print_position(GstElement *pipeline)
{
  gint64 pos, len;
  static gboolean seek_done = FALSE;

  if (gst_element_query_position(pipeline, GST_FORMAT_TIME, &pos)
    && gst_element_query_duration(pipeline, GST_FORMAT_TIME, &len)
    ) {
      g_print("Time: %" GST_TIME_FORMAT " / %" GST_TIME_FORMAT "\r",
        GST_TIME_ARGS(pos), GST_TIME_ARGS(len));
      
      if (!seek_done && pos > 10 * GST_SECOND) {
        g_print ("\nReached 10s, performing seek...\n");
        gst_element_seek_simple(pipeline
                      , GST_FORMAT_TIME
                      , GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT, 35 * GST_SECOND);
        seek_done = TRUE;
      }
  }

  return TRUE;
}

static gboolean bus_call(GstBus *bus, GstMessage *msg, gpointer data)
{
  GMainLoop *loop = (GMainLoop *)data;

  switch (GST_MESSAGE_TYPE(msg))
  {
  case GST_MESSAGE_EOS:
    DD("End of stream");
    g_main_loop_quit(loop);
    break;
  case GST_MESSAGE_ERROR: {
    gchar *debug;
    GError *error;

    gst_message_parse_error(msg, &error, &debug);
    g_free(debug);

    DD("Error: %s", error->message);
    g_error_free(error);

    g_main_loop_quit(loop);

    break;
  }
  default:
    break;
  }

  return TRUE;
}

static void on_pad_added(GstElement * element, GstPad *pad, gpointer data)
{
  GstPad *sinkpad;
  GstElement *decoder = (GstElement *)data;

  DD("Dynamic pad created. linking demuxer/decoder");

  sinkpad = gst_element_get_static_pad(decoder, "sink");

  gst_pad_link(pad, sinkpad);
  gst_object_unref(sinkpad);
}

// gst-launch-1.0 filesrc location=~/Videos/audio.ogg ! decodebin ! audioconvert ! autoaudiosink 
// gst-launch-1.0 filesrc location=~/Videos/audio.ogg ! oggdemux ! vorbisdec ! audioconvert ! autoaudiosink
int main(int argc, char *argv[])
{
  GMainLoop *loop;

  GstElement *pipeline, *source, *demuxer, *decoder, *conv, *sink;
  GstBus *bus;
  guint bus_watch_id;

  gst_init(&argc, &argv);

  loop = g_main_loop_new(NULL, FALSE);

  if (argc != 2) {
    g_printerr("Usage %s <Ogg/Vorbix filename>\n", argv[0]);
    return -1;
  }

  pipeline  = gst_pipeline_new("audio-player");
  source    = gst_element_factory_make("filesrc", "file-source");
  demuxer   = gst_element_factory_make("oggdemux", "ogg-demuxer");
  decoder   = gst_element_factory_make("vorbisdec", "vorbis-decoder");
  conv      = gst_element_factory_make("audioconvert", "convert");
  sink      = gst_element_factory_make("autoaudiosink", "audio-output");

  if (!pipeline || !source || !demuxer || !decoder || !conv || !sink) {
    DE("One element could not be created. Exiting");
    return -1;
  }

  g_object_set(G_OBJECT(source), "location", argv[1], NULL);

  bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
  bus_watch_id = gst_bus_add_watch(bus, bus_call, loop);
  gst_object_unref(bus);

  gst_bin_add_many(GST_BIN(pipeline),
                    source, demuxer, decoder, conv, sink, NULL);
  
  gst_element_link(source, demuxer);
  gst_element_link_many(decoder, conv, sink, NULL);
  g_signal_connect(demuxer, "pad-added", G_CALLBACK(on_pad_added), decoder);

  DD("Now playing: %s", argv[1]);
  gst_element_set_state(pipeline, GST_STATE_PLAYING);

  g_timeout_add(100, (GSourceFunc)cb_print_position, pipeline);

  DD("Running...");
  g_main_loop_run(loop);

  DD("Deleting pipeline");
  gst_object_unref(GST_OBJECT(pipeline));
  g_source_remove(bus_watch_id);
  g_main_loop_unref(loop);

  return 0;
}
