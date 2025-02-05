 #define LOG_TAG      "prac-07-meta"
 #define LOG_NDEBUG   0
 #include <logging.h>

#include <gst/gst.h>

static void print_one_tag(const GstTagList * list, const gchar * tag, gpointer user_data)
{
  int i, num;

  num = gst_tag_list_get_tag_size(list, tag);
  for (i = 0; i < num; i++) {
    const GValue *val;

    val = gst_tag_list_get_value_index(list, tag, i);
    if (G_VALUE_HOLDS_STRING(val)) {
      DD("\t1. %20s : %s", tag, g_value_get_string(val));
    } else if (G_VALUE_HOLDS_UINT(val)) {
      DD("\t2. %20s : %u", tag, g_value_get_uint(val));
    } else if (G_VALUE_HOLDS_DOUBLE(val)) {
      DD("\t3. %20s : %g", tag, g_value_get_double(val));
    } else if (G_VALUE_HOLDS_BOOLEAN(val)) {
      DD("\t4. %20s : %s", tag, g_value_get_boolean(val) ? "true" : "false");
    } else if (GST_VALUE_HOLDS_BUFFER(val)) {
      GstBuffer * buf = gst_value_get_buffer(val);
      guint buffer_size = gst_buffer_get_size(buf);

      DD("\t5. %20s : buffer of size %u", tag, buffer_size);
    } else if (GST_VALUE_HOLDS_DATE_TIME(val)) {
      GstDateTime *dt = g_value_get_boxed(val);
      gchar * dt_str = gst_date_time_to_iso8601_string(dt);

      DD("\t6. %20s : %s", tag, dt_str);
    } else {
      DD("\t7. %20s : tag of type '%s'", tag, G_VALUE_TYPE_NAME(val));
    }
  }
}

static void on_new_pad(GstElement * dec, GstPad *pad, GstElement * fakesink)
{
  GstPad *sinkpad;

  sinkpad = gst_element_get_static_pad(fakesink, "sink");
  if (!gst_pad_is_linked(sinkpad)) {
    if (gst_pad_link(pad, sinkpad) != GST_PAD_LINK_OK) {
      DE("Failed to link pads!");
      return;
    } else {
      DD("Dynamic pad created. linking demuxer/decoder");
    }
  }

  gst_object_unref(sinkpad);
}

int main(int argc, char *argv[])
{
  GstElement *pipe, *dec, *sink;
  GstMessage *msg;
  gchar *uri;

  gst_init(&argc, &argv);

  if (argc < 2) {
    printf("Usage: %s FILE or URI\n", argv[0]);
    return -1;
  }

  if (gst_uri_is_valid(argv[1])) {
    uri = g_strdup(argv[1]);
  } else {
    uri = gst_filename_to_uri(argv[1], NULL);
  }

  pipe = gst_pipeline_new("pipeline");

  dec = gst_element_factory_make("uridecodebin", NULL);
  g_object_set(dec, "uri", uri, NULL);
  gst_bin_add(GST_BIN(pipe), dec);

  sink = gst_element_factory_make("fakesink", NULL);
  gst_bin_add(GST_BIN(pipe), sink);

  g_signal_connect(dec, "pad-added", G_CALLBACK(on_new_pad), sink);

  gst_element_set_state(pipe, GST_STATE_PAUSED);

  while(TRUE) {
    GstTagList *tags = NULL;

    msg = gst_bus_timed_pop_filtered(GST_ELEMENT_BUS(pipe), GST_CLOCK_TIME_NONE,
            GST_MESSAGE_ASYNC_DONE | GST_MESSAGE_TAG | GST_MESSAGE_ERROR);
    
    if (GST_MESSAGE_TYPE(msg) != GST_MESSAGE_TAG) {
      DD("Break : %s", GST_MESSAGE_TYPE_NAME(msg));
      break;
    }

    gst_message_parse_tag(msg, &tags);

    DD("Got tags from element %s:", GST_OBJECT_NAME(msg->src));
    gst_tag_list_foreach(tags, print_one_tag, NULL);
    DD("");
    gst_tag_list_unref(tags);
    gst_message_unref(msg);
  }

  if (GST_MESSAGE_TYPE(msg) == GST_MESSAGE_ERROR) {
    GError *err = NULL;

    gst_message_parse_error(msg, &err, NULL);
    DE("Got error: %s", err->message);
    g_error_free(err);
  }

  gst_message_unref(msg);
  gst_element_set_state(pipe, GST_STATE_NULL);
  gst_object_unref(pipe);
  g_free(uri);

  return 0;
}
