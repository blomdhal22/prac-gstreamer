 #define LOG_TAG      "prac-init"
 #define LOG_NDEBUG   0
 #include <logging.h>

#include <stdio.h>
#include <gst/gst.h>

#if 0
int main(int argc, char *argv[])
{
    gboolean silent = FALSE;
    gchar *savefile = NULL;
    GOptionContext *ctx;
    GError *err = NULL;
    GOptionEntry entries[] = {
        { "silent", 's', 0, G_OPTION_ARG_NONE, &silent,
        "do not output status information", NULL },
        { "output", 'o', 0, G_OPTION_ARG_STRING, &savefile,
        "save xml representation of pipeline to FILE and exit", "FILE" },
        { NULL }
    };

    ctx = g_option_context_new ("- Your application");
    g_option_context_add_main_entries (ctx, entries, NULL);
    g_option_context_add_group (ctx, gst_init_get_option_group ());
    if (!g_option_context_parse (ctx, &argc, &argv, &err)) {
        g_print ("Failed to initialize: %s\n", err->message);
        g_clear_error (&err);
        g_option_context_free (ctx);
        return 1;
    }
    g_option_context_free (ctx);

    printf ("Run me with --help to see the Application options appended.\n");

    return 0;
}
#endif


#if 1
int main(int argc, char *argv[])
{
    const gchar *nano_str;
    guint major, minor, micro, nano;

    gst_init(&argc, &argv);
    gst_version(&major, &minor, &micro, &nano);

    if (nano == 1)
        nano_str = "(CVS)";
    else if (nano == 2)
        nano_str = "(Prerelease)";
    else
        nano_str = "";
    
    DD("This program is linked against GStreamer %d.%d.%d %s",
          major, minor, micro, nano_str);

    return 0;
}
#endif 