#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "component.h"
#include "power.h"
#include "resistance.h"

void text_entry_callback(GtkWidget *text_entry);
void submit_button_clicked();
void quit_button_clicked();

static GtkWidget *text_entries[6]; 
static float voltage, ohm_1, ohm_2, ohm_3;
static unsigned char connection;
static int components;
static GtkWidget *result_label;
int main(int argc, char *argv[])
{

    GtkWidget *window;
    GtkWidget *parent_vbox;
    GtkWidget *child_hbox_1, *child_hbox_2, *child_hbox_9, *child_hbox_10;
    GtkWidget *menu;
    GtkWidget *menu_bar;
    GtkWidget *root_menu;
    GtkWidget *menu_item_quit;
    GtkWidget *menu_vbox;
    GtkWidget *submit_button;
    GtkWidget *quit_button;
    //GtkWidget *result_label;
    GtkWidget *child_vbox[6];
    GtkWidget *label_widgets[6]; 
    //GtkWidget *text_entries[6];
    const unsigned char *labels[6] = {"Voltage", "Connection", "Components",
                             "Ohm 1", "Ohm 2", "Ohm 3"}; 
    int i;
    
    //Initializes ???
    gtk_init (&argc, &argv);

    //Create a new window 
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_usize (GTK_WIDGET (window), 500, 500);
    gtk_window_set_title (GTK_WINDOW (window), "Electro");
    gtk_signal_connect (GTK_OBJECT (window), "delete_event",
		(GtkSignalFunc) gtk_main_quit, NULL);

    //Create main vertical box which will contain eight hboxes.
    //TRUE=Give children equql space. 0=zero pixels between children.
    parent_vbox = gtk_vbox_new(FALSE, 0);
    //gtk_box_pack_start(GTK_BOX (
    gtk_container_add(GTK_CONTAINER (window), parent_vbox);    
  //  gtk_box_pack_start(GTK_BOX (parent_vbox), window, 10, 10, 10);
    gtk_widget_show(parent_vbox);

    //Create child_hbox_1 which will contain a menu.
    child_hbox_1 = gtk_hbox_new(FALSE, 0);
    //gtk_container_add(GTK_CONTAINER (parent_vbox), child_hbox_1);
    gtk_box_pack_start(GTK_BOX (parent_vbox), child_hbox_1, 0, 0, 0);
    gtk_widget_show(child_hbox_1);

    //Create the menu.
    menu = gtk_menu_new ();
    menu_item_quit = gtk_menu_item_new_with_label ("Quit");
    gtk_menu_append (GTK_MENU (menu), menu_item_quit);
    //Connect the menuitem to a callback-function.
    gtk_signal_connect(GTK_OBJECT (menu_item_quit), "activate", 
    		GTK_SIGNAL_FUNC (quit_button_clicked), "Quit Button Menu");
    gtk_widget_show (menu_item_quit);
    //Create the initially visible menu-button.  
    root_menu = gtk_menu_item_new_with_label ("Menu");
    gtk_widget_show (root_menu);
    //Create the submenu containing the menu-items.
    gtk_menu_item_set_submenu (GTK_MENU_ITEM (root_menu), menu);
    menu_vbox = gtk_vbox_new (FALSE, 0);

    gtk_container_add (GTK_CONTAINER (child_hbox_1), menu_vbox);
    gtk_widget_show (menu_vbox);

    menu_bar = gtk_menu_bar_new ();
    gtk_box_pack_start(GTK_BOX (menu_vbox), menu_bar, FALSE, FALSE, 2);
    gtk_widget_show (menu_bar);

    gtk_menu_bar_append (GTK_MENU_BAR (menu_bar), root_menu);
//////////////////////////////////////////////////////////////////////////////////////////////////////
    //Set hbox that holds six vboxes each containing a label and a text-entry.
    child_hbox_2 = gtk_hbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER (parent_vbox), child_hbox_2);
    gtk_widget_show(child_hbox_2);

    for(i = 0; i < 6; i++) {
	child_vbox[i] = gtk_vbox_new(FALSE, 0);
        gtk_container_add(GTK_CONTAINER (child_hbox_2), child_vbox[i]);
        //gtk_box_pack_start(GTK_BOX (child_hbox_2), child_vbox[i], 0, 0, 20);
        gtk_widget_show(child_vbox[i]);

        label_widgets[i] = gtk_label_new(labels[i]);
        gtk_box_pack_start(GTK_BOX (child_vbox[i]), label_widgets[i], 0, 0, 20);
        gtk_widget_show(label_widgets[i]);

        text_entries[i] = gtk_entry_new();
        gtk_container_add(GTK_CONTAINER (child_vbox[i]), text_entries[i]);
	//gtk_box_pack_start(GTK_BOX (label_widgets[i]), child_vbox[i], 0, 0, 20);
        gtk_signal_connect(GTK_OBJECT (text_entries[i]), "activate", 
                          GTK_SIGNAL_FUNC (text_entry_callback), text_entries[i]);
               //gtk_entry_set_max_length(text_entries[i], 70);
        gtk_widget_show(text_entries[i]);
    }
/////////////////////////////////////////////////////////////////////////////////////////////////////    
    //Create box that holds a label(result).
    child_hbox_9 = gtk_hbox_new(TRUE, 0);
    gtk_container_add(GTK_CONTAINER (parent_vbox), child_hbox_9);
    gtk_widget_show(child_hbox_9);
    
    result_label = gtk_label_new("Result");
    //gtk_container_add(GTK_CONTAINER (child_hbox_9), result_label);
    gtk_box_pack_start(GTK_BOX (child_hbox_9), result_label, 0, 0, 0);
    gtk_widget_show(result_label);

    //Create box that holds buttons
    child_hbox_10 = gtk_hbox_new(TRUE, 0);
    gtk_container_add(GTK_CONTAINER (parent_vbox), child_hbox_10);
    gtk_widget_show(child_hbox_10);

    submit_button = gtk_button_new_with_label ("Submit");
    gtk_signal_connect_object (GTK_OBJECT (submit_button), "clicked",
        	GTK_SIGNAL_FUNC (submit_button_clicked), "Submit Button");
    gtk_container_add(GTK_CONTAINER(child_hbox_10) , submit_button);  
    gtk_widget_show (submit_button);

    quit_button = gtk_button_new_with_label ("Quit");
    gtk_signal_connect_object (GTK_OBJECT (quit_button), "clicked",
        	GTK_SIGNAL_FUNC (quit_button_clicked), "Quit Button");
    gtk_container_add(GTK_CONTAINER(child_hbox_10) , quit_button);  
    
    gtk_widget_show (quit_button);

  
    //Display window
    gtk_widget_show (window);
    //Start loop.
    gtk_main ();

    return(0);
}

//Get text from text-entries and check if value is valid.
//If not, set error message in text entry.
void text_entry_callback(GtkWidget *text_entry)
{
        const gchar *entry_text;
	int i;
        char str[20];
	if(text_entry == text_entries[0]) {
		entry_text = gtk_entry_get_text(GTK_ENTRY(text_entry));
		strcpy(str, entry_text);
		printf("Voltage %f %s\n", atof(str), entry_text);
		if(atof(str) <= 0) {
			gtk_entry_set_text(GTK_ENTRY (text_entry), "Not valid");
			voltage = 0;
		}
		else
			voltage = atof(str);
	}
	else if(text_entry == text_entries[1]) {
		entry_text = gtk_entry_get_text(GTK_ENTRY(text_entry));
		printf("Connection %s\n", entry_text);
		if(strcmp(entry_text, "S") == 0) {
			connection = 'S'; //*entry_text;
		}
		else if(strcmp(entry_text, "P") == 0) {
			connection = 'P'; //*entry_text;
		}
		else {
			gtk_entry_set_text(GTK_ENTRY (text_entry), "Not valid");	
			connection = ' ';
		}
	}	
	else if(text_entry == text_entries[2]) {
		entry_text = gtk_entry_get_text(GTK_ENTRY(text_entry));
		printf("Components %i %s\n", atoi(entry_text), entry_text);
		if(atoi(entry_text) > 3 || atoi(entry_text) < 1) {
			gtk_entry_set_text(GTK_ENTRY (text_entry), "Not valid");
			components = 0;
		}
		else 
			components = atoi(entry_text);
	}	
	else if(text_entry == text_entries[3]) {
		entry_text = gtk_entry_get_text(GTK_ENTRY(text_entry));
		strcpy(str, entry_text);
		printf("Ohm 1 %f %s\n", atof(str), entry_text);
		if(atof(str) <= 0) {
			gtk_entry_set_text(GTK_ENTRY (text_entry), "Not valid");
			ohm_1 = 0;
		}
		else
			ohm_1 = atof(str);
	}	
	else if(text_entry == text_entries[4]) {
		entry_text = gtk_entry_get_text(GTK_ENTRY(text_entry));
		strcpy(str, entry_text);
		printf("Ohm 2 %f %s\n", atof(str), entry_text);
		if(atof(str) <= 0) {
			gtk_entry_set_text(GTK_ENTRY (text_entry), "Not valid");
			ohm_2 = 0;
		}
		else
			ohm_2 = atof(str);
	}	
	else if(text_entry == text_entries[5]) {
		entry_text = gtk_entry_get_text(GTK_ENTRY(text_entry));
		strcpy(str, entry_text);
		printf("Ohm 3 %f %s\n", atof(str), entry_text);
		if(atof(str) <= 0) {
			gtk_entry_set_text(GTK_ENTRY (text_entry), "Not valid");
			ohm_3 = 0;
		}
		else
			ohm_3 = atof(str);
	}
}

void submit_button_clicked()
{ 
	float resistance, components_array[3];
	if(voltage > 0 && (connection == 'S' || connection == 'P') && components > 0
           && components <= 3 && ohm_1 && ohm_2 && ohm_3) {
		
		components_array[0] = ohm_1;
         	components_array[1] = ohm_2;
		components_array[2] = ohm_3;
		resistance = calc_resistance(components, connection, components_array);

       		 gtk_label_set_text((GtkLabel *) result_label, "Text");
		printf("Submit\n");
        }
	else
		gtk_label_set_text((GtkLabel *) result_label, "Invalid input.");
}

void quit_button_clicked()
{
	printf("Quit \n");
	gtk_main_quit();
}
