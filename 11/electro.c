#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "component.h"
#include "power.h"
#include "resistance.h"
#include "power.h"
#include "component.h"

void text_entry_callback(GtkWidget *text_entry);
void submit_button_clicked();
void quit_button_clicked();

static GtkWidget *text_entries[6]; 
static float voltage;
static unsigned char connection;
static int nr_of_components;
static float *components; //Allocated to the size of nr_of_components.
static GtkWidget *result_label;

int main(int argc, char *argv[])
{

    GtkWidget *window;
    GtkWidget *main_vbox;
    GtkWidget *sub_hbox_1, *sub_hbox_2, *sub_hbox_9, *sub_hbox_10;
    GtkWidget *menu;
    GtkWidget *menu_bar;
    GtkWidget *root_menu;
    GtkWidget *menu_item_quit;
    GtkWidget *menu_vbox;
    GtkWidget *submit_button;
    GtkWidget *quit_button;
    GtkWidget *sub_vbox[6];
    GtkWidget *label_widgets[6]; 
    const unsigned char *labels[6] = {"Voltage", "Connection", "Components",
                             "Ohm 1", "Ohm 2", "Ohm 3"}; 
    int i;
    
    //Initializes ???
    gtk_init (&argc, &argv);

    //Create a new window 
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_usize (GTK_WIDGET (window), 500, 260);
    gtk_window_set_title (GTK_WINDOW (window), "Electro");
    gtk_signal_connect (GTK_OBJECT (window), "delete_event",
		(GtkSignalFunc) gtk_main_quit, NULL);

    //Create main vertical box which will contain sub-boxes.
    //TRUE=Give sub-boxes equal space. 0=zero pixels between sub-boxes.
    main_vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER (window), main_vbox);    
    gtk_widget_show(main_vbox);

    //Create sub_hbox_1 which will contain a menu.
    sub_hbox_1 = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX (main_vbox), sub_hbox_1, 0, 0, 20);
    gtk_widget_show(sub_hbox_1);

    //Create the menu.
    menu = gtk_menu_new ();
    menu_item_quit = gtk_menu_item_new_with_label ("Quit");
    gtk_menu_append (GTK_MENU (menu), menu_item_quit);

    //Connect the menuitem to a callback-function.
    gtk_signal_connect(GTK_OBJECT (menu_item_quit), "activate", 
    		GTK_SIGNAL_FUNC (quit_button_clicked), "Quit Button Menu");
    gtk_widget_show (menu_item_quit);

    //Create the menu-button.  
    root_menu = gtk_menu_item_new_with_label ("Menu");
    gtk_widget_show (root_menu);

    //Create the submenu containing the menu-items.
    gtk_menu_item_set_submenu (GTK_MENU_ITEM (root_menu), menu);
    menu_vbox = gtk_vbox_new (FALSE, 0);

    gtk_container_add (GTK_CONTAINER (sub_hbox_1), menu_vbox);
    gtk_widget_show (menu_vbox);

    menu_bar = gtk_menu_bar_new ();
    gtk_box_pack_start(GTK_BOX (menu_vbox), menu_bar, FALSE, FALSE, 2);
    gtk_widget_show (menu_bar);

    gtk_menu_bar_append (GTK_MENU_BAR (menu_bar), root_menu);
//////////////////////////////////////////////////////////////////////////////////////////////////////
    //Set hbox that holds six vboxes each containing a label and a text-entry.
    sub_hbox_2 = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX (main_vbox), sub_hbox_2, 0, 0, 0);
    gtk_widget_show(sub_hbox_2);

    for(i = 0; i < 6; i++) {
	sub_vbox[i] = gtk_vbox_new(FALSE, 0);
        gtk_container_add(GTK_CONTAINER (sub_hbox_2), sub_vbox[i]);
        gtk_widget_show(sub_vbox[i]);

        label_widgets[i] = gtk_label_new(labels[i]);
        gtk_box_pack_start(GTK_BOX (sub_vbox[i]), label_widgets[i], 0, 0, 0);
        gtk_widget_show(label_widgets[i]);

        text_entries[i] = gtk_entry_new();
        gtk_container_add(GTK_CONTAINER (sub_vbox[i]), text_entries[i]);
        gtk_signal_connect(GTK_OBJECT (text_entries[i]), "activate", 
                          GTK_SIGNAL_FUNC (text_entry_callback), text_entries[i]);
        gtk_widget_show(text_entries[i]);
    }
/////////////////////////////////////////////////////////////////////////////////////////////////////    
    //Create box that holds a label(result).
    sub_hbox_9 = gtk_hbox_new(TRUE, 0);
    gtk_box_pack_start(GTK_BOX (main_vbox), sub_hbox_9, 0, 0, 50);
    gtk_widget_show(sub_hbox_9);
    
    result_label = gtk_label_new("Result");
    gtk_box_pack_start(GTK_BOX (sub_hbox_9), result_label, 0, 0, 0);
    gtk_widget_show(result_label);

    //Create box that holds buttons
    sub_hbox_10 = gtk_hbox_new(TRUE, 0);
    gtk_box_pack_start(GTK_BOX (main_vbox), sub_hbox_10, 0, 0, 0);
    gtk_widget_show(sub_hbox_10);

    submit_button = gtk_button_new_with_label ("Submit");
    gtk_signal_connect_object (GTK_OBJECT (submit_button), "clicked",
        	GTK_SIGNAL_FUNC (submit_button_clicked), "Submit Button");
    //gtk_container_add(GTK_CONTAINER(sub_hbox_10) , submit_button); 
    gtk_box_pack_start(GTK_BOX (sub_hbox_10), submit_button, 0, 0, 0); 
    gtk_widget_show (submit_button);

    quit_button = gtk_button_new_with_label ("Quit");
    gtk_signal_connect_object (GTK_OBJECT (quit_button), "clicked",
        	GTK_SIGNAL_FUNC (quit_button_clicked), "Quit Button");
    gtk_container_add(GTK_CONTAINER(sub_hbox_10) , quit_button);  
    
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
		if(atof(str) <= 0) {
			gtk_entry_set_text(GTK_ENTRY (text_entry), "0");
			voltage = 0;
		}
		else
			voltage = atof(str);
	}
	else if(text_entry == text_entries[1]) {
		entry_text = gtk_entry_get_text(GTK_ENTRY(text_entry));
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
		if(atoi(entry_text) > 3 || atoi(entry_text) < 1) {
			gtk_entry_set_text(GTK_ENTRY (text_entry), "0");
			nr_of_components = 0;
			free(components);
		}
		else {
			nr_of_components = atoi(entry_text);
			components = (float *) malloc(nr_of_components * sizeof(float));
			if(!components) 
				printf("Error allocating memory.\n");
		}
	}	
	else if(text_entry == text_entries[3]) {
		entry_text = gtk_entry_get_text(GTK_ENTRY(text_entry));
		strcpy(str, entry_text);
		if(atof(str) <= 0) {
			gtk_entry_set_text(GTK_ENTRY (text_entry), "0");
			*components = 0;
		}
		else
			*components = atof(str);
	}	
	else if(text_entry == text_entries[4]) {
		entry_text = gtk_entry_get_text(GTK_ENTRY(text_entry));
		strcpy(str, entry_text);
		if(atof(str) <= 0) {
			gtk_entry_set_text(GTK_ENTRY (text_entry), "0");
			*(components + 1) = 0;
		}
		else
			*(components + 1) = atof(str);
	}	
	else if(text_entry == text_entries[5]) {
		entry_text = gtk_entry_get_text(GTK_ENTRY(text_entry));
		strcpy(str, entry_text);
		if(atof(str) <= 0) {
			gtk_entry_set_text(GTK_ENTRY (text_entry), "0");
			*(components + 2) = 0;
		}
		else
			*(components + 2) = atof(str);
	}
}

void submit_button_clicked()
{ 
	float resistance, power; 
        float replacement_comp[3] = {0.00, 0.00, 0.00};
	int nr_of_replacement_comp;
        char str[120] = {0};
	//Checking inserted values before submitting.
	if(voltage > 0 && (connection == 'S' || connection == 'P') && nr_of_components > 0
           && nr_of_components <= 3) {
		
		resistance = calc_resistance(nr_of_components, connection, components);
		power = calc_power_r(voltage, resistance);		
                nr_of_replacement_comp = e_resistance(resistance, replacement_comp);                 
                 sprintf(str,"Ersättingsresistans: %.2f\nEffekt: %.2f\nErsättingsresistanser: %.0f, %.0f, %.0f\n", 
                 resistance, power, replacement_comp[0], replacement_comp[1], replacement_comp[2]);
       		 gtk_label_set_text((GtkLabel *) result_label, str);
		printf("Submit\n");
		free(components);
        }
	else {
		gtk_label_set_text((GtkLabel *) result_label, "Invalid input.");
		free(components);
	}
}

void quit_button_clicked()
{
	printf("Quit \n");
	gtk_main_quit();
}
