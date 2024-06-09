 #define LOG_TAG      "prac-02-element"
 #define LOG_NDEBUG   0
 #include <logging.h>

#include <gst/gst.h>

int main (int argc, char *argv[])
{
    GstElement *pipeline;
    GstElement *source, *filter, *sink;

    /* init */
    gst_init (&argc, &argv);

    /* create pipeline */
    pipeline = gst_pipeline_new ("my-pipeline");

    /* create elements */
    source = gst_element_factory_make ("fakesrc", "source123");
    filter = gst_element_factory_make ("identity", "filter123");
    sink = gst_element_factory_make ("fakesink", "sink123");

    g_return_val_if_fail(source != NULL, -1);
    g_return_val_if_fail(filter != NULL, -1);
    g_return_val_if_fail(sink != NULL, -1);

    /* must add elements to pipeline before linking them */
    gst_bin_add_many (GST_BIN (pipeline), source, filter, sink, NULL);

    /* link */
    if (!gst_element_link_many(source, filter, sink, NULL)) {
        g_warning ("Failed to link elements!");
    }

    return 0;
}

#if 0
int main (int   argc,char *argv[])
{
  GstElementFactory *factory;

  /* init GStreamer */
  gst_init (&argc, &argv);

  /* get factory */
  factory = gst_element_factory_find ("fakesrc");
  if (!factory) {
    g_print ("You don't have the 'fakesrc' element installed!\n");
    return -1;
  }

  /* display information */
  g_print ("The '%s' element is a member of the category %s.\n"
           "Description: %s\n"
           "Author: %s\n",
           gst_plugin_feature_get_name (GST_PLUGIN_FEATURE (factory)),
           gst_element_factory_get_metadata (factory, GST_ELEMENT_METADATA_KLASS),
           gst_element_factory_get_metadata (factory, GST_ELEMENT_METADATA_DESCRIPTION),
           gst_element_factory_get_metadata (factory, GST_ELEMENT_METADATA_AUTHOR)
           );

  gst_object_unref (GST_OBJECT (factory));

  return 0;
}
#endif


#if 0
int main(int argc, char *argv[])
{
    GstElement *element;
    gchar *name;

    gst_init(&argc, &argv);

    element = gst_element_factory_make("fakesrc", "source123");

    // g_object_get(G_OBJECT(element), "name", &name, NULL);
    g_object_get(GST_OBJECT(element), "name", &name, NULL);
    DD("The name of the element is '%s'", name);
    g_free(name);
    
    g_object_get(GST_OBJECT(element), "last-message", &name, NULL);
    DD("last-message '%s'", name);

    g_free(name);
    gst_object_unref(GST_OBJECT(element));

    return 0;
}
#endif

#if 0
int main(int argc, char *argv[])
{
    GstElement *element;
    GstElementFactory *factory;

    gst_init(&argc, &argv);

    factory = gst_element_factory_find("fakesrc");
    g_return_val_if_fail(factory != NULL, -1);

    element = gst_element_factory_create(factory, "source");    // as unique name
    g_return_val_if_fail(element != NULL, -1);

    gst_object_unref(GST_OBJECT(element));
    gst_object_unref(GST_OBJECT(factory));

    return 0;
}
#endif

#if 0
int main(int argc, char *argv[])
{
    GstElement *element;

    gst_init(&argc, &argv);

    element = gst_element_factory_make("fakesrc", "source");
    if (element == NULL) {
        DE("Failed to create element of type 'fakesrc'");
        return -1;
    }

    gst_object_unref(GST_OBJECT(element));

    return 0;
}
#endif
