#include <cairo.h>
#include <gtk/gtk.h>
#include <time.h>


static char buffer[256];

/******************************
*  把buffer显示到窗口中

*  每次画窗口时调用

*/
static gboolean
on_expose_event(GtkWidget *widget,GdkEventExpose *event,gpointer data)
{
	cairo_t *cr;

	cr = gdk_cairo_create(widget->window);

	cairo_move_to(cr, 30, 30);
	cairo_show_text(cr, buffer);

	cairo_destroy(cr);

	return FALSE;
}

gint delete_event( GtkWidget *widget,GdkEvent  *event,gpointer   data )
{
	gtk_main_quit ();
	return FALSE;
}

 
/******************************
*  把当前时间打印到buffer中，并且重画窗口

*  每次timeout后调用，即每秒调用一次

*/
static gboolean
time_handler(GtkWidget *widget)
{
	if (widget->window == NULL) return FALSE;

  	time_t curtime;
  	struct tm *loctime;

  	curtime = time(NULL);
  	loctime = localtime(&curtime);
  	strftime(buffer, 256, "%T", loctime);

  	gtk_widget_queue_draw(widget);
  	return TRUE;
}


int
main (int argc, char *argv[])
{

  	GtkWidget *window;
  	GtkWidget *darea;
  	GtkWidget *button;
  	GtkWidget *label;
  	GtkWidget *table;
 	/* 创建一个新窗口 */
 	gtk_init(&argc, &argv);

  	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  	/* 设置窗口标题 */
  	gtk_window_set_title (GTK_WINDOW (window), "time");

  	/* 为delete_event设置一个立即退出GTK的处理函数。 */
  	g_signal_connect (G_OBJECT (window), "delete_event",G_CALLBACK (delete_event), NULL);

  	/* 设置窗口的边框宽度。 */
  	gtk_container_set_border_width (GTK_CONTAINER (window), 20);

	/* 创建一个2x2的表 */
    	table = gtk_table_new (3, 2, TRUE);
    	/* 将表放进主窗口 */
    	gtk_container_add (GTK_CONTAINER (window), table);

  	darea = gtk_drawing_area_new();
  	gtk_container_add(GTK_CONTAINER (window), darea);
    	/* 创建一个label */
    	label = gtk_label_new("The time is:");
    	/* 将label放进主窗口 */
    	gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 0, 2);
    	gtk_widget_show (label);
    	/* 创建"Quit"按钮 */
    	button = gtk_button_new_with_label ("Quit");
    	/* 当这个按钮被点击时，我们调用 "delete_event" 函数接着
     	* 程序就退出了 */
    	g_signal_connect (G_OBJECT (button), "clicked",
                      G_CALLBACK (delete_event), NULL);
    	/* 将退出按钮插入表的下面两个象限 */
    	gtk_table_attach_defaults (GTK_TABLE (table), button, 0, 2, 2, 3);
  	g_signal_connect(darea, "expose-event",G_CALLBACK(on_expose_event), NULL);   // 每次画窗口时的callback
  	g_signal_connect(window, "destroy",G_CALLBACK(gtk_main_quit), NULL);
	gtk_table_attach_defaults (GTK_TABLE (table), darea, 1, 2, 0, 1);
  	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  	gtk_window_set_default_size(GTK_WINDOW(window), 170, 150);

  	g_timeout_add(1000, (GSourceFunc) time_handler, (gpointer) window);
  	gtk_widget_show_all(window);
  	time_handler(window);

  	gtk_main();

  	return 0;
}



