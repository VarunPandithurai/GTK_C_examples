/*
* This Example didn't work
*/
#include <gtk/gtk.h>
#include "tictactoe.h"

enum {
  TICTACTOE_SIGNAL,
  LAST_SIGNAL
};
guint
tictactoe_get_type ()
{
  static guint ttt_type = 0;

  if (!ttt_type)
    {
      GtkTypeInfo ttt_info =
      {
        "Tictactoe",
        sizeof (Tictactoe),
        sizeof (TictactoeClass),
        // (GtkClassInitFunc) tictactoe_class_init,
        // (GtkObjectInitFunc) tictactoe_init,
        // (GtkArgSetFunc) NULL,
        // (GtkArgGetFunc) NULL
      };

      ttt_type = gtk_type_unique (gtk_vbox_get_type (), &ttt_info);
    }

  return ttt_type;
}
static gint tictactoe_signals[LAST_SIGNAL] = { 0 };
      
    GtkWidget*
    tictactoe_new ()
    {
      return GTK_WIDGET ( gtk_type_new (tictactoe_get_type ()));
    }

    void           
    tictactoe_clear (Tictactoe *ttt)
    {
      int i,j;

      for (i=0;i<3;i++)
        for (j=0;j<3;j++)
          {
            gtk_signal_handler_block_by_data (GTK_OBJECT(ttt->buttons[i][j]), ttt);
            gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (ttt->buttons[i][j]),
                                         FALSE);
            gtk_signal_handler_unblock_by_data (GTK_OBJECT(ttt->buttons[i][j]), ttt);
          }
    }

    static void
    tictactoe_toggle (GtkWidget *widget, Tictactoe *ttt)
    {
      int i,k;

      static int rwins[8][3] = { { 0, 0, 0 }, { 1, 1, 1 }, { 2, 2, 2 },
                                 { 0, 1, 2 }, { 0, 1, 2 }, { 0, 1, 2 },
                                 { 0, 1, 2 }, { 0, 1, 2 } };
      static int cwins[8][3] = { { 0, 1, 2 }, { 0, 1, 2 }, { 0, 1, 2 },
                                 { 0, 0, 0 }, { 1, 1, 1 }, { 2, 2, 2 },
                                 { 0, 1, 2 }, { 2, 1, 0 } };

      int success, found;

      for (k=0; k<8; k++)
        {
          success = TRUE;
          found = FALSE;

          for (i=0;i<3;i++)
            {
              success = success && 
                GTK_TOGGLE_BUTTON(ttt->buttons[rwins[k][i]][cwins[k][i]])->active;
              found = found ||
                ttt->buttons[rwins[k][i]][cwins[k][i]] == widget;
            }
          
          if (success && found)
            {
              gtk_signal_emit (GTK_OBJECT (ttt), 
                               tictactoe_signals[TICTACTOE_SIGNAL]);
              break;
            }
        }
    }



/* Invoked when a row, column or diagonal is completed */
void
win (GtkWidget *widget, gpointer data)
{
  g_print ("Yay!\n");
  tictactoe_clear (TICTACTOE (widget));
}

int 
main (int argc, char *argv[])
{
  GtkWidget *window;
  GtkWidget *ttt;
  
  gtk_init (&argc, &argv);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  
  gtk_window_set_title (GTK_WINDOW (window), "Aspect Frame");
  
  gtk_signal_connect (GTK_OBJECT (window), "destroy",
                      GTK_SIGNAL_FUNC (gtk_exit), NULL);
  
  gtk_container_set_border_width (GTK_CONTAINER (window), 10);

  /* Create a new Tictactoe widget */
  ttt = tictactoe_new ();
  gtk_container_add (GTK_CONTAINER (window), ttt);
  gtk_widget_show (ttt);

  /* And attach to its "tictactoe" signal */
  gtk_signal_connect (GTK_OBJECT (ttt), "tictactoe",
                      GTK_SIGNAL_FUNC (win), NULL);

  gtk_widget_show (window);
  
  gtk_main ();
  
  return 0;
}