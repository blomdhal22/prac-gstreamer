 #define LOG_TAG      "prac-webcam1"
 #define LOG_NDEBUG   0
 #include <logging.h>

#if 0
#include <gst/gst.h>

// gst-launch-1.0 -e v4l2src device=/dev/video0 ! videoconvert ! videorate ! video/x-raw,framerate=30/1 ! x264enc ! mp4mux ! filesink location=output.mp4

int main(int argc, char *argv[]) {
    GstElement *pipeline, *source, *converter, *rate, *encoder, *muxer, *sink;
    GstBus *bus;
    GstMessage *msg;
    GstStateChangeReturn ret;

    /* GStreamer 초기화 */
    gst_init(&argc, &argv);

    /* 파이프라인 생성 */
    pipeline = gst_pipeline_new("webcam-pipeline");
    source = gst_element_factory_make("v4l2src", "source");
    converter = gst_element_factory_make("videoconvert", "converter");
    rate = gst_element_factory_make("videorate", "rate");
    encoder = gst_element_factory_make("x264enc", "encoder");
    muxer = gst_element_factory_make("mp4mux", "muxer");
    sink = gst_element_factory_make("filesink", "sink");

    if (!pipeline || !source || !converter || !rate || !encoder || !muxer || !sink) {
        g_error("Failed to create elements.");
        return -1;
    }

    /* 파일명 설정 */
    g_object_set(G_OBJECT(sink), "location", "output.mp4", NULL);

    /* 파이프라인에 요소 추가 */
    gst_bin_add_many(GST_BIN(pipeline), source, converter, rate, encoder, muxer, sink, NULL);

    /* 요소를 연결 */
    if (!gst_element_link_many(source, converter, rate, encoder, muxer, sink, NULL)) {
        g_error("Failed to link elements.");
        return -1;
    }

    /* 웹캠에서 캡처할 비디오 포맷 설정 */
    g_object_set(G_OBJECT(source), "device", "/dev/video0", NULL);

    /* 프레임 속도를 30fps로 설정 */
    g_object_set(G_OBJECT(rate), "max-rate", 30, NULL);

    /* 파이프라인 상태를 PLAYING으로 변경 */
    ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        g_error("Failed to start pipeline.");
        return -1;
    }

    /* 메시지 버스 생성 */
    bus = gst_element_get_bus(pipeline);
    msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_ERROR | GST_MESSAGE_EOS);

    /* 메시지 처리 */
    if (msg != NULL) {
        GError *err;
        gchar *debug_info;
        switch (GST_MESSAGE_TYPE(msg)) {
            case GST_MESSAGE_ERROR:
                gst_message_parse_error(msg, &err, &debug_info);
                g_printerr("Error received from element %s: %s\n", GST_OBJECT_NAME(msg->src), err->message);
                g_printerr("Debugging information: %s\n", debug_info ? debug_info : "none");
                g_clear_error(&err);
                g_free(debug_info);
                break;
            case GST_MESSAGE_EOS:
                g_print("End-Of-Stream reached.\n");
                break;
            default:
                g_printerr("Unexpected message received.\n");
                break;
        }
        gst_message_unref(msg);
    }

    /* 파이프라인 상태를 NULL로 변경하여 정리 */
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(GST_OBJECT(pipeline));
    return 0;
}
#endif

#include <gst/gst.h>

int main(int argc, char *argv[]) {
    GstElement *pipeline, *source, *converter, *rate, *encoder, *muxer, *sink;
    GstBus *bus;
    GstMessage *msg;

    /* GStreamer 초기화 */
    gst_init(&argc, &argv);

    DD("%ld", __LINE__);

    /* 파이프라인 생성 */
    pipeline = gst_pipeline_new("webcam-pipeline");
    source = gst_element_factory_make("v4l2src", "source");
    converter = gst_element_factory_make("videoconvert", "converter");
    rate = gst_element_factory_make("videorate", "rate");
    encoder = gst_element_factory_make("x264enc", "encoder");
    muxer = gst_element_factory_make("mp4mux", "muxer");
    sink = gst_element_factory_make("filesink", "sink");

    if (!pipeline || !source || !converter || !rate || !encoder || !muxer || !sink) {
        g_error("Failed to create elements.");
        return -1;
    }

    DD("%ld", __LINE__);

    /* 파일명 설정 */
    g_object_set(G_OBJECT(sink), "location", "output.mp4", NULL);

    DD("%ld", __LINE__);

    /* 파이프라인에 요소 추가 */
    gst_bin_add_many(GST_BIN(pipeline), source, converter, rate, encoder, muxer, sink, NULL);

    DD("%ld", __LINE__);

    /* 요소를 연결 */
    if (!gst_element_link_many(source, converter, rate, encoder, muxer, sink, NULL)) {
        g_error("Failed to link elements.");
        return -1;
    }

    DD("%ld", __LINE__);

    /* 웹캠에서 캡처할 비디오 포맷 설정 */
    g_object_set(G_OBJECT(source), "device", "/dev/video0", NULL);

    DD("%ld", __LINE__);

    /* 프레임 속도를 30fps로 설정 */
    // g_object_set(G_OBJECT(rate), "max-rate", 30, NULL);

    DD("%ld", __LINE__);

    /* 파이프라인 상태를 PLAYING으로 변경 */
    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    /* 메시지 버스 생성 */
    bus = gst_element_get_bus(pipeline);

    DD("%ld", __LINE__);
    
    msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_ERROR | GST_MESSAGE_EOS);

    DD("%ld", __LINE__);

    /* 메시지 처리 */
    if (msg != NULL) {
        GError *err;
        gchar *debug_info;
        switch (GST_MESSAGE_TYPE(msg)) {
            case GST_MESSAGE_ERROR:
                gst_message_parse_error(msg, &err, &debug_info);
                g_printerr("Error received from element %s: %s\n", GST_OBJECT_NAME(msg->src), err->message);
                g_printerr("Debugging information: %s\n", debug_info ? debug_info : "none");
                g_clear_error(&err);
                g_free(debug_info);
                break;
            case GST_MESSAGE_EOS:
                g_print("End-Of-Stream reached.\n");
                break;
            default:
                g_printerr("Unexpected message received.\n");
                break;
        }
        gst_message_unref(msg);
    }

    /* 파이프라인 상태를 NULL로 변경하여 정리 */
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(GST_OBJECT(pipeline));
    return 0;
}
