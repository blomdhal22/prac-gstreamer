 #define LOG_TAG      "myapp-00"
 #define LOG_NDEBUG   0
 #include <logging.h>

#include <gst/gst.h>

#include <glib.h>

// webcam preview
static gboolean bus_call(GstBus *bus, GstMessage *msg, gpointer data) {
    GMainLoop *loop = (GMainLoop *) data;

    switch (GST_MESSAGE_TYPE(msg)) {
        case GST_MESSAGE_EOS:
            g_print("End of stream\n");
            g_main_loop_quit(loop);
            break;
        case GST_MESSAGE_ERROR: {
            gchar *debug;
            GError *error;

            gst_message_parse_error(msg, &error, &debug);
            g_free(debug);

            g_printerr("Error: %s\n", error->message);
            g_error_free(error);

            g_main_loop_quit(loop);

            break;
        }
        default:
            break;
    }

    return TRUE;
}

int main(int argc, char *argv[]) {
    GMainLoop *loop;
    GstElement *pipeline, *source, *sink, *dec;
    GstBus *bus;
    guint bus_watch_id;

    /* GStreamer 초기화 */
    gst_init(&argc, &argv);
    loop = g_main_loop_new(NULL, FALSE);

    /* 파이프라인 및 요소 생성 */
    pipeline = gst_pipeline_new("webcam-preview");
    source = gst_element_factory_make("v4l2src", "source");
    dec = gst_element_factory_make("jpegdec", "dec");
    sink = gst_element_factory_make("autovideosink", "sink");

    if (!pipeline || !source || !sink || !dec) {
        g_printerr("One element could not be created. Exiting.\n");
        return -1;
    }

    /* 파이프라인에 요소 추가 */
    gst_bin_add_many(GST_BIN(pipeline), source, dec, sink, NULL);

    /* 소스 요소 설정 */
    g_object_set(source, "device", "/dev/video4", NULL);

    /* 소스와 싱크 요소 연결 */
    if (!gst_element_link_many(source, dec, sink, NULL)) {
        g_printerr("Elements could not be linked. Exiting.\n");
        return -1;
    }

    /* 버스 가져오기 */
    bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
    bus_watch_id = gst_bus_add_watch(bus, bus_call, loop);
    gst_object_unref(bus);

    /* 파이프라인 실행 */
    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    /* 메인 루프 실행 */
    g_print("Running...\n");
    g_main_loop_run(loop);

    /* 메인 루프 종료 후 정리 */
    g_print("Stopping...\n");
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(GST_OBJECT(pipeline));
    g_source_remove(bus_watch_id);
    g_main_loop_unref(loop);

    return 0;
}




#if 0
// gst-launch-1.0 videotestsrc ! autovideosink audiotestsrc ! autoaudiosink
int main(int argc, char *argv[])
{
  GstElement *pipeline;
  GstBus *bus;
  GstMessage *msg;

  gst_init (&argc, &argv);

  // pipeline = gst_parse_launch("videotestsrc pattern=11 ! autovideosink", NULL);
  // pipeline = gst_parse_launch("videotestsrc ! autovideosink audiotestsrc ! autoaudiosink", NULL); 
  // pipeline = gst_parse_launch("videotestsrc ! video/x-raw,width=320,height=240 "
  //                             "! timeoverlay halignment=center valignment=center font-desc=\"Sans, 32\" "
  //                             "! autovideosink"
  //                             , NULL);
  // pipeline = gst_parse_launch("videotestsrc ! videoconvert ! tee name=t ! queue ! autovideosink t. ! queue ! autovideosink"
  //                             , NULL);
  pipeline = gst_parse_launch("v4l2src device=/dev/video0 ! jpegdec ! xvimagesink", NULL);

  gst_element_set_state (pipeline, GST_STATE_PLAYING);

  bus = gst_element_get_bus (pipeline);
  msg = gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_ERROR | GST_MESSAGE_EOS);

  if (msg != NULL) {
    GError * err;
    gchar *debug_info;

    switch (GST_MESSAGE_TYPE(msg))
    {
    case GST_MESSAGE_ERROR:
      gst_message_parse_error(msg, &err, &debug_info);
      DE("Error received from element %s: %s",GST_OBJECT_NAME(msg->src), err->message);
      DE("Debugging information: %s", debug_info ? debug_info : "none");
      g_clear_error(&err);
      g_free(debug_info);
      break;
    case GST_MESSAGE_EOS:
      DD("End of Stream reached");
      break;
    default:
      DE("Unexpected message received");
      break;
    }

    gst_message_unref(msg);
  }

  gst_object_unref (bus);
  gst_element_set_state (pipeline, GST_STATE_NULL);
  gst_object_unref (pipeline);

  return 0;
}
#endif

#if 0
// $ gst-launch-1.0 videotestsrc ! videoconvert ! autovideosink
int main(int argc, char *argv[])
{
  GstElement *pipeline, *source, *convert, *sink;
  gboolean ret = FALSE;
  GstStateChangeReturn stat;
  GstBus *bus;
  GstMessage *msg;

  // init
  gst_init(&argc, &argv);

  // make element
  source = gst_element_factory_make("videotestsrc", "source");
  convert = gst_element_factory_make("videoconvert", "convert");
  sink = gst_element_factory_make("autovideosink", "sink");

  // pipe line
  pipeline = gst_pipeline_new("pipeline");

  if (!pipeline || !source || !convert || !sink) {
    DE("Not all elements could be created.");
    return -1;
  }

  // build pipeline
  gst_bin_add_many(GST_BIN(pipeline), source, convert, sink, NULL);
  // ret = gst_element_link(source, sink);
  ret = gst_element_link_many(source, convert, sink, NULL);
  if (ret != TRUE) {
    DE("Elements could not be linked");
    gst_object_unref(pipeline);
    return -1;
  }

  // set prop
  g_object_set(source, "pattern", 0, NULL);

  // start play
  stat = gst_element_set_state(pipeline, GST_STATE_PLAYING);
  if (stat  == GST_STATE_CHANGE_FAILURE) {
    DE("Unable to set the pipeline to the playing state");
    gst_object_unref(pipeline);
    return -1;
  }

  bus = gst_element_get_bus(pipeline);
  msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE,
        GST_MESSAGE_ERROR | GST_MESSAGE_EOS);
  
  if (msg != NULL) {
    GError * err;
    gchar *debug_info;

    switch (GST_MESSAGE_TYPE(msg))
    {
    case GST_MESSAGE_ERROR:
      gst_message_parse_error(msg, &err, &debug_info);
      DE("Error received from element %s: %s",GST_OBJECT_NAME(msg->src), err->message);
      DE("Debugging information: %s", debug_info ? debug_info : "none");
      g_clear_error(&err);
      g_free(debug_info);
      break;
    case GST_MESSAGE_EOS:
      DD("End of Stream reached");
      break;
    default:
      DE("Unexpected message received");
      break;
    }

    gst_message_unref(msg);
  }

  // free
  gst_object_unref(bus);
  gst_element_set_state(pipeline, GST_STATE_NULL);
  gst_object_unref(pipeline);

  return 0;
}
#endif

#if 0
// $ gst-launch-1.0 videotestsrc ! autovideosink
int main(int argc, char *argv[])
{
  GstElement *pipeline, *source, *sink;
  gboolean ret = FALSE;
  GstStateChangeReturn stat;
  GstBus *bus;
  GstMessage *msg;

  // init
  gst_init(&argc, &argv);

  // make element
  source = gst_element_factory_make("videotestsrc", "source");
  sink = gst_element_factory_make("autovideosink", "sink");

  // pipe line
  pipeline = gst_pipeline_new("pipeline");

  if (!pipeline || !source || !sink) {
    DE("Not all elements could be created.");
    return -1;
  }

  // build pipeline
  gst_bin_add_many(GST_BIN(pipeline), source, sink, NULL);
  ret = gst_element_link(source, sink);
  if (ret != TRUE) {
    DE("Elements could not be linked");
    gst_object_unref(pipeline);
    return -1;
  }

  // set prop
  g_object_set(source, "pattern", 0, NULL);

  // start play
  stat = gst_element_set_state(pipeline, GST_STATE_PLAYING);
  if (stat  == GST_STATE_CHANGE_FAILURE) {
    DE("Unable to set the pipeline to the playing state");
    gst_object_unref(pipeline);
    return -1;
  }

  bus = gst_element_get_bus(pipeline);
  msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE,
        GST_MESSAGE_ERROR | GST_MESSAGE_EOS);
  
  if (msg != NULL) {
    GError * err;
    gchar *debug_info;

    switch (GST_MESSAGE_TYPE(msg))
    {
    case GST_MESSAGE_ERROR:
      gst_message_parse_error(msg, &err, &debug_info);
      DE("Error received from element %s: %s",GST_OBJECT_NAME(msg->src), err->message);
      DE("Debugging information: %s", debug_info ? debug_info : "none");
      g_clear_error(&err);
      g_free(debug_info);
      break;
    case GST_MESSAGE_EOS:
      DD("End of Stream reached");
      break;
    default:
      DE("Unexpected message received");
      break;
    }

    gst_message_unref(msg);
  }

  // free
  gst_object_unref(bus);
  gst_element_set_state(pipeline, GST_STATE_NULL);
  gst_object_unref(pipeline);

  return 0;
}
#endif
