 #define LOG_TAG      "prac-04-bus"
 #define LOG_NDEBUG   0
 #include <logging.h>

#include <gst/gst.h>

static GMainLoop * loop;

static gboolean my_bus_callback(GstBus * bus, GstMessage * message, gpointer data)
{
    DD("Got %s message", GST_MESSAGE_TYPE_NAME (message));

    switch (GST_MESSAGE_TYPE(message)) {
        case GST_MESSAGE_ERROR: {
            GError * err;
            gchar *debug;

            gst_message_parse_error(message, &err, &debug);
            g_print("Error: %s\n", err->message);
            g_error_free(err);
            g_free(debug);

            g_main_loop_quit(loop);

            break;
        }
        case GST_MESSAGE_EOS:
            g_main_loop_quit(loop);
            break;
        default:
            break;

    }

    return TRUE;
}

int main(int argc, char *argv[])
{
    GstElement * pipeline;
    GstBus * bus;
    guint bus_watch_id;

    gst_init(&argc, &argv);

    pipeline = gst_pipeline_new("my_pipeline");

    bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
    bus_watch_id = gst_bus_add_watch(bus, my_bus_callback, NULL);
    gst_object_unref(GST_OBJECT(bus));

    /* [...] */

    loop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(loop);

    /* clean up */
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
    g_source_remove(bus_watch_id);
    g_main_loop_unref(loop);


    return 0;
}

