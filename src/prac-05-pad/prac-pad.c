 #define LOG_TAG      "prac-05-pad"
 #define LOG_NDEBUG   0
 #include <logging.h>

#include <gst/gst.h>

static gboolean
link_elements_with_filter2 (GstElement *element1, GstElement *element2)
{
  gboolean link_ok;
  GstCaps *caps;

  caps = gst_caps_new_full (
      gst_structure_new ("video/x-raw",
             "width", G_TYPE_INT, 384,
             "height", G_TYPE_INT, 288,
             "framerate", GST_TYPE_FRACTION, 25, 1,
             NULL),
      gst_structure_new ("video/x-bayer",
             "width", G_TYPE_INT, 384,
             "height", G_TYPE_INT, 288,
             "framerate", GST_TYPE_FRACTION, 25, 1,
             NULL),
      NULL);

  link_ok = gst_element_link_filtered (element1, element2, caps);
  gst_caps_unref (caps);

  if (!link_ok) {
    g_warning ("Failed to link element1 and element2!");
  }

  return link_ok;
}


static gboolean
link_elements_with_filter (GstElement *element1, GstElement *element2)
{
  gboolean link_ok;
  GstCaps *caps;

  caps = gst_caps_new_simple ("video/x-raw",
          "format", G_TYPE_STRING, "I420",
          "width", G_TYPE_INT, 384,
          "height", G_TYPE_INT, 288,
          "framerate", GST_TYPE_FRACTION, 25, 1,
          NULL);

  link_ok = gst_element_link_filtered (element1, element2, caps);
  gst_caps_unref (caps);

  if (!link_ok) {
    g_warning ("Failed to link element1 and element2!");
  }

  return link_ok;
}

static void read_video_props(GstCaps * caps)
{
    gint width, height;
    const GstStructure * str;

    g_return_if_fail(gst_caps_is_fixed(caps));

    str = gst_caps_get_structure(caps, 0);
    if (!gst_structure_get_int(str, "width", &width) ||
        !gst_structure_get_int(str, "height", &height)) {
            g_print("No width/height available");
            return;
    }

    g_print ("The video size of this set of capabilities is %dx%d\n",
    width, height);
}

static void cb_new_pad(GstElement * element, GstPad *pad, gpointer data)
{
    gchar *name;

    name = gst_pad_get_name(pad);
    DD("A new pad %s was created", name);
    g_free(name);
}

int main(int argc, char *argv[])
{
    GstElement *bin, *sink;
    GstPad *pad;

    gst_init(&argc, &argv);

    sink = gst_element_factory_make("fakesink", "sink");
    bin = gst_bin_new("mybin");
    gst_bin_add(GST_BIN(bin), sink);

    pad = gst_element_get_static_pad(sink, "sink");
    gst_element_add_pad(bin, gst_ghost_pad_new("sink", pad));
    gst_object_unref(GST_OBJECT(pad));

    /* [..] */

    return 0;
}



#if 0
int main(int argc, char *argv[])
{
    GstElement *pipeline, *source, *demux;
    GMainLoop *loop;

    gst_init(&argc, &argv);

    pipeline = gst_pipeline_new("my_pipeline");
    source = gst_element_factory_make("filesrc", "source");
    g_object_set(source, "location", argv[1], NULL);
    demux = gst_element_factory_make("oggdemux", "demuxer");

    gst_bin_add_many(GST_BIN(pipeline), source, demux, NULL);
    gst_element_link_pads(source, "src", demux, "sink");

    g_signal_connect(demux, "pad-added", G_CALLBACK(cb_new_pad), NULL);

    gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_PLAYING);
    loop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(loop);

    /* [...] */

    return 0;
}
#endif