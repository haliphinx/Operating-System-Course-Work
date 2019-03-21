#include <stdlib.h>
#include <gtk/gtk.h>
#include <time.h>
gint delete_event( GtkWidget *widget,GdkEvent  *event,gpointer   data )
{
	gtk_main_quit ();
	return FALSE;
}

typedef struct PACKED         //定义一个cpu occupy的结构体
{
	char name[20];      //定义一个char类型的数组名name有20个元素
	unsigned int user; 	//用户态时间，不包含nice值为负的进程
	unsigned int nice; 	//nice值为负的进程时间
	unsigned int system;	//核态时间
	unsigned int idle; 	//除了io等待外其他的等待时间
	unsigned int iowait;	//io等待时间
	unsigned int irq;	//硬中断时间
	unsigned int softirq;	//软中断时间
	unsigned int stealstolen;	//虚拟环境时间
	unsigned int guest;		//其他用户时间
}CPU_OCCUPY;
static gboolean cpuUse(GtkWidget* cpuLabel);
float getCpuoOccupy ();
int main (int argc, char *argv[]){
        GtkWidget *window= NULL,*cpuLabel = NULL;
	GtkWidget *button;
  	GtkWidget *table;
        /* Initialize GTK+ */
        gtk_init (&argc, &argv);

        /* Create the main window */
        window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
        gtk_container_set_border_width (GTK_CONTAINER (window), 20);

        gtk_widget_set_usize(window,300,100);
        gtk_window_set_title (GTK_WINDOW (window), "cpu ");
        gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER);
        gtk_window_set_resizable(GTK_WINDOW(window),FALSE);
        gtk_widget_realize (window);
	/* 为delete_event设置一个立即退出GTK的处理函数。 */
  	g_signal_connect (G_OBJECT (window), "delete_event",G_CALLBACK (delete_event), NULL);
	/* 创建一个3x2的表 */
    	table = gtk_table_new (2, 1, TRUE);
    	/* 将表放进主窗口 */
    	gtk_container_add (GTK_CONTAINER (window), table);


    	/* 创建"Quit"按钮 */
    	button = gtk_button_new_with_label ("Quit");
    	/* 当这个按钮被点击时，我们调用 "delete_event" 函数接着
     	* 程序就退出了 */
    	g_signal_connect (G_OBJECT (button), "clicked",
                      G_CALLBACK (delete_event), NULL);
    	/* 将退出按钮插入表的下面两个象限 */
    	gtk_table_attach_defaults (GTK_TABLE (table), button, 0, 1, 1, 2);
        //cpu使用率
        cpuLabel = gtk_label_new(NULL);
        gtk_table_attach_defaults (GTK_TABLE (table), cpuLabel, 0, 1, 0, 1);
        cpuUse(cpuLabel);
        g_timeout_add(2000,(GSourceFunc)cpuUse,(gpointer)cpuLabel);
        /* Enter the main loop */
        gtk_widget_show_all (window);
        gtk_main ();
        return 0;
}
static gboolean cpuUse(GtkWidget* cpuLabel){
        char labelArray[64];
        sprintf(labelArray,"cpu使用率:%%%.2f",getCpuoOccupy());
        gtk_label_set(GTK_LABEL(cpuLabel),labelArray);
        return TRUE;
}

float getCpuoOccupy (){
        CPU_OCCUPY occupy;
        unsigned long all1, all2,idle1, idle2;
        float cpuUse = 0.0;
   	FILE *fp;
        char buff[256];

        fp = fopen ("/proc/stat", "r");
        fgets (buff, sizeof(buff), fp);

        sscanf (buff, "%s %u %u %u %u %u %u %u %u %u", occupy.name, &occupy.user, &occupy.nice,&occupy.system, &occupy.idle,
    										 &occupy.iowait,&occupy.irq,&occupy.softirq,&occupy.stealstolen,&occupy.guest);
   	all1 = occupy.user + occupy.nice + occupy.system + occupy.idle + occupy.iowait
                + occupy.irq + occupy.softirq + occupy.stealstolen + occupy.guest;
        idle1 = occupy.idle;

   	//第二次获取
   	sleep(1);
   	rewind (fp);
        fgets (buff, sizeof(buff), fp);
        fclose(fp);
        sscanf (buff, "%s %u %u %u %u %u %u %u %u %u", occupy.name, &occupy.user, &occupy.nice,&occupy.system, &occupy.idle,
                                                                                        &occupy.iowait,&occupy.irq,&occupy.softirq,&occupy.stealstolen,&occupy.guest);
   	all2 =  occupy.user + occupy.nice + occupy.system + occupy.idle  + occupy.iowait
                + occupy.irq + occupy.softirq + occupy.stealstolen + occupy.guest;
        idle2 = occupy.idle;

        //计算使用率
        if(all2 - all1 == 0){
                cpuUse = 100.00;
        }else{
                cpuUse = (float)(all2 - all1 - (idle2 - idle1))/(all2 - all1)*100;
        }
        return cpuUse;
}



