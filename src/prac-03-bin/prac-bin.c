 #define LOG_TAG      "prac-02-element"
 #define LOG_NDEBUG   0
 #include <logging.h>

#include <gst/gst.h>

int main(int argc, char *argv[])
{
    GstElement * player;

    /* init */
    gst_init(&argc, &argv);

    /* create player */
    // player = gst_element_factory_make("oggvorbisplayer", "player");
    player = gst_element_factory_make("playbin", "player");

    /* set the source audio file */
    g_object_set(player, "location", "helloworld.ogg", NULL);

    /* start playback */
    gst_element_set_state(GST_ELEMENT(player), GST_STATE_PLAYING);

    return 0;
}

#if 0
// g_return_val_if_fail(source != NULL, -1);
int main (int argc, char *argv[])
{
    GstElement *bin;
    GstElement *pipeline;
    GstElement *source, *sink;

    /* init */
    gst_init(&argc, &argv);

    /* create */
    pipeline = gst_pipeline_new("my pipeline");
    bin = gst_bin_new("my_bin");
    source = gst_element_factory_make("fakesrc", "source");
    sink = gst_element_factory_make("fakesink", "sink");

    /* Firat add the elements to the bin */
    gst_bin_add_many(GST_BIN(bin), source, sink, NULL);
    /* add the bin to the pipeline */
    gst_bin_add(GST_BIN(pipeline), bin);

    /* link the elements */
    gst_element_link(source, sink);


    return 0;
}
#endif
