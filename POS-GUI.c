#include <gtk/gtk.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <sys/stat.h>

typedef struct item
{ // A structured data type is a new data type created by the user to meet their requirements
	int num;
	char name[50];
	int stock;
	float price;
} item; // Created a sructured data type: item, to neatly store data from POS Data.csv for quick access

int csv_to_array(item[], FILE *);
void array_to_csv(item[], char[], int);
char *time_to_string(char *);
void intialize_arr_val(float[][3], float, int);
int pass_check(const char *pass, const char *correct_pass);
void update_data_buffer();
void update_cart_view();
void tendered_update();
void backup_stock();
void undo_stock_changes();

GtkWidget *window;
GtkWidget *fixed;
GtkWidget *itemlabel;
GtkWidget *itemdisplay;
GtkWidget *billlabel;
GtkWidget *BillDisplay;
GtkWidget *cartlabel;
GtkWidget *cartdisplay;
GtkWidget *cashlabel;
GtkWidget *quanlabel;
GtkWidget *itemn;
GtkWidget *cashier;
GtkWidget *itemnum;
GtkWidget *quant;
GtkWidget *totallabel;
GtkWidget *dislabel;
GtkWidget *GSTlabel;
GtkWidget *discountent;
GtkWidget *GSTent;
GtkWidget *addtocart_button;
GtkWidget *totalview;
GtkWidget *tenderedlabel;
GtkWidget *tenderedentry;
GtkWidget *msgdisplay;
GtkWidget *changelabel;
GtkWidget *generate_button;
GtkWidget *startWindow;
GtkWidget *startButton;
GtkWidget *passEntry;
GtkWidget *scroll1;
GtkWidget *scroll2;
GtkWidget *scroll3;
GtkWidget *scroll4;
GtkWidget *ChoiceWindow;
GtkWidget *CheckoutButton;
GtkWidget *buttonfixed;
GtkWidget *labelx;
GtkWidget *EditButton;
GtkWidget *fixed4;
GtkWidget *labely;
GtkWidget *abortButton;
GtkWidget *quitButton;
GtkWidget *EditWindow;
GtkWidget *editfixed;
GtkWidget *quit2;
GtkWidget *EditNumber;
GtkWidget *ItemStock;
GtkWidget *AddButton;
GtkWidget *RemoveButton;
GtkWidget *EditPrice;
GtkWidget *EditItemName;
GtkWidget *UpdateButton;
GtkWidget *scroll5;
GtkWidget *msgdisplay1;

GtkTextBuffer *billbuffer;
GtkTextBuffer *cartbuffer;
GtkTextBuffer *message;
GtkTextBuffer *totalbuffer;
GtkTextBuffer *databasebuffer;

GtkEntryBuffer *tenderedbuffer;
GtkEntryBuffer *discountbuf;
GtkEntryBuffer *cashiername;
GtkEntryBuffer *itemnobuffer;
GtkEntryBuffer *quanbuffer;
GtkEntryBuffer *GSTbuf;

GtkBuilder *builder;

char cwd[256], bildir[256], data_dir[256], buffer[300], file[32], cashierStr[20], timeStr[20], choice;

float customer_cart[100][3], sub_total = 0, total = 0, bill_total, GST, GST_p, discount, discount_p, tendered, change;
int search, quantity, found, address, name_size, unique_items, z, spot_found, r;

FILE *bil_fptr, *POS_Data_fptr;
item data_array[200] = {0};
int indexVar, customer_no, i, data_array_stock_backup[200];

int main(int argc, char **argv)
{
	gtk_init(&argc, &argv);

	builder = gtk_builder_new_from_file("builder.glade");
	window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
	startWindow = GTK_WIDGET(gtk_builder_get_object(builder, "startWindow"));
	EditWindow = GTK_WIDGET(gtk_builder_get_object(builder, "EditWindow"));
	ChoiceWindow = GTK_WIDGET(gtk_builder_get_object(builder, "ChoiceWindow"));
	g_signal_connect(EditWindow, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(ChoiceWindow, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(startWindow, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	gtk_builder_connect_signals(builder, NULL);

	window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
	fixed = GTK_WIDGET(gtk_builder_get_object(builder, "fixed"));
	itemlabel = GTK_WIDGET(gtk_builder_get_object(builder, "itemlabel"));
	itemdisplay = GTK_WIDGET(gtk_builder_get_object(builder, "itemdisplay"));
	billlabel = GTK_WIDGET(gtk_builder_get_object(builder, "billlabel"));
	BillDisplay = GTK_WIDGET(gtk_builder_get_object(builder, "BillDisplay"));
	cartlabel = GTK_WIDGET(gtk_builder_get_object(builder, "cartlabel"));
	cartdisplay = GTK_WIDGET(gtk_builder_get_object(builder, "cartdisplay"));
	cashlabel = GTK_WIDGET(gtk_builder_get_object(builder, "cashlabel"));
	quanlabel = GTK_WIDGET(gtk_builder_get_object(builder, "quanlabel"));
	itemn = GTK_WIDGET(gtk_builder_get_object(builder, "itemn"));
	cashier = GTK_WIDGET(gtk_builder_get_object(builder, "cashier"));
	itemnum = GTK_WIDGET(gtk_builder_get_object(builder, "itemnum"));
	quant = GTK_WIDGET(gtk_builder_get_object(builder, "quant"));
	totallabel = GTK_WIDGET(gtk_builder_get_object(builder, "totallabel"));
	dislabel = GTK_WIDGET(gtk_builder_get_object(builder, "dislabel"));
	GSTlabel = GTK_WIDGET(gtk_builder_get_object(builder, "GSTlabel"));
	discountent = GTK_WIDGET(gtk_builder_get_object(builder, "discountent"));
	GSTent = GTK_WIDGET(gtk_builder_get_object(builder, "GSTent"));
	addtocart_button = GTK_WIDGET(gtk_builder_get_object(builder, "addtocart"));
	totalview = GTK_WIDGET(gtk_builder_get_object(builder, "totalview"));
	tenderedlabel = GTK_WIDGET(gtk_builder_get_object(builder, "tenderedlabel"));
	tenderedentry = GTK_WIDGET(gtk_builder_get_object(builder, "tenderedentry"));
	msgdisplay = GTK_WIDGET(gtk_builder_get_object(builder, "msgdisplay"));
	changelabel = GTK_WIDGET(gtk_builder_get_object(builder, "changelabel"));
	generate_button = GTK_WIDGET(gtk_builder_get_object(builder, "generate"));
	passEntry = GTK_WIDGET(gtk_builder_get_object(builder, "passEntry"));
	startButton = GTK_WIDGET(gtk_builder_get_object(builder, "startButton"));
	startWindow = GTK_WIDGET(gtk_builder_get_object(builder, "startWindow"));
	scroll1 = GTK_WIDGET(gtk_builder_get_object(builder, "scroll1"));
	scroll2 = GTK_WIDGET(gtk_builder_get_object(builder, "scroll2"));
	scroll3 = GTK_WIDGET(gtk_builder_get_object(builder, "scroll3"));
	scroll4 = GTK_WIDGET(gtk_builder_get_object(builder, "scroll4"));
	ChoiceWindow = GTK_WIDGET(gtk_builder_get_object(builder, "ChoiceWindow"));
	CheckoutButton = GTK_WIDGET(gtk_builder_get_object(builder, "CheckoutButton"));
	buttonfixed = GTK_WIDGET(gtk_builder_get_object(builder, "buttonfixed"));
	labelx = GTK_WIDGET(gtk_builder_get_object(builder, "labelx"));
	EditButton = GTK_WIDGET(gtk_builder_get_object(builder, "EditButton"));
	fixed4 = GTK_WIDGET(gtk_builder_get_object(builder, "fixed4"));
	labely = GTK_WIDGET(gtk_builder_get_object(builder, "labely"));
	abortButton = GTK_WIDGET(gtk_builder_get_object(builder, "abortButton"));
	quitButton = GTK_WIDGET(gtk_builder_get_object(builder, "quitButton"));
	editfixed = GTK_WIDGET(gtk_builder_get_object(builder, "editfixed"));
	quit2 = GTK_WIDGET(gtk_builder_get_object(builder, "quit2"));
	EditNumber = GTK_WIDGET(gtk_builder_get_object(builder, "EditNumber"));
	ItemStock = GTK_WIDGET(gtk_builder_get_object(builder, "ItemStock"));
	AddButton = GTK_WIDGET(gtk_builder_get_object(builder, "AddButton"));
	RemoveButton = GTK_WIDGET(gtk_builder_get_object(builder, "RemoveButton"));
	EditPrice = GTK_WIDGET(gtk_builder_get_object(builder, "EditPrice"));
	EditItemName = GTK_WIDGET(gtk_builder_get_object(builder, "EditItemName"));
	UpdateButton = GTK_WIDGET(gtk_builder_get_object(builder, "UpdateButton"));
	scroll5 = GTK_WIDGET(gtk_builder_get_object(builder, "scroll5"));
	msgdisplay1 = GTK_WIDGET(gtk_builder_get_object(builder, "msgdisplay1"));

	billbuffer = GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "billbuffer"));
	cartbuffer = GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "cartbuffer"));
	message = GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "message"));
	totalbuffer = GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "totalbuffer"));
	databasebuffer = GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "databasebuffer"));

	tenderedbuffer = GTK_ENTRY_BUFFER(gtk_builder_get_object(builder, "tenderedbuffer"));
	discountbuf = GTK_ENTRY_BUFFER(gtk_builder_get_object(builder, "discountbuf"));
	cashiername = GTK_ENTRY_BUFFER(gtk_builder_get_object(builder, "cashiername"));
	itemnobuffer = GTK_ENTRY_BUFFER(gtk_builder_get_object(builder, "itemnobuffer"));
	quanbuffer = GTK_ENTRY_BUFFER(gtk_builder_get_object(builder, "quanbuffer"));
	GSTbuf = GTK_ENTRY_BUFFER(gtk_builder_get_object(builder, "GSTbuf"));

	gtk_widget_hide(window);
	gtk_widget_show(startWindow);
	if (getcwd(cwd, 300) == NULL)
	{
		perror("getcwd() error"); // using unistd.h, getcwd() function to Get current working directory
		return 1; /// perror just adds error message after our string, when the program is run after compilation
	}
	strcpy(data_dir, cwd);
	strcat(data_dir, "\\POS Data.csv");
	strcpy(bildir, cwd);
	strcat(bildir, "\\Customer Bills");
	mkdir(bildir);					// Makes folder for Customer Bills
	strcpy(data_dir, cwd);
	strcat(data_dir, "\\POS Data.csv");

	POS_Data_fptr = fopen(data_dir, "r+");

	indexVar = csv_to_array(data_array, POS_Data_fptr);
	if (indexVar == -1)
	{
		printf("CSV Format error\n");
		fclose(POS_Data_fptr);
		return 1;
	}

	GdkRGBA color, color2;
	color.red = 0.7;
	color.green = 0.7;
	color.blue = 0.77;
	color.alpha = 1;
	color2.red = 0.5;
	color2.green = 0.7;
	color2.blue = 0.77;
	color2.alpha = 1;
	gtk_widget_override_background_color(window, GTK_STATE_FLAG_NORMAL, &color);
	gtk_main();
	array_to_csv(data_array, data_dir, indexVar);
	return EXIT_SUCCESS;
}

G_MODULE_EXPORT void on_cashier_changed(GtkEntry *uselessvar3)
{
	strcpy(cashierStr, gtk_entry_buffer_get_text(cashiername));
	gchar str[40];
	strcat(str, "Welcome to POS, ");
	strcat(str, cashierStr);
	gtk_text_buffer_set_text(message, (gchar *)str, -1);
}

G_MODULE_EXPORT void on_addtocart_clicked(GtkButton *b)
{
	gchar str[40];
	sprintf(str, "Messages will be displayed here:\n");
	gtk_text_buffer_set_text(message, str, -1);
	search = atoi(gtk_entry_buffer_get_text(itemnobuffer));
	quantity = atoi(gtk_entry_buffer_get_text(quanbuffer));

	for (address = 0, found = 0; address < indexVar && found != 1; address++)
		if (search == data_array[address].num) // Finding the item number in the data array
			found = 1;
	address--; // address-- due to for loop incrementing address by 1 before ending

	sub_total = 0, spot_found = 0;

	if (found == 1 && quantity > 0)
	{
		if (data_array[address].stock < quantity)
		{
			gtk_text_buffer_insert_at_cursor(message, "Insufficient stock.\n", -1);
			return;
		}
		total += data_array[address].price * quantity;
		sub_total = data_array[address].price * quantity;
		data_array[address].stock -= quantity;

		// Finding an empty index in the customer_cart array, this is inorder to prevent dublicate printing.
		for (z = 0; z < 100 && spot_found == 0; z++)
			if (address == customer_cart[z][0])
				spot_found = 1;
		if (spot_found != 1)
			for (z = 0; z < 100 && spot_found != 1; z++)
				if (customer_cart[z][0] == -1)
					spot_found = 1;
		z--;
		/// customer_cart[i][0] gets the indexVar of the item corresponding to the indexVar in the data dir, and customer_cart[i][1] refers to the quantity of that item
		/// searching if item indexVar in datadir matches the item indexVar stored in 100 different products
		/// spot found = 0 is only possible when number of unique items exceeds 100

		customer_cart[z][0] = address;
		customer_cart[z][1] += quantity;
		customer_cart[z][2] += sub_total;

		/// this works for if the item was already in the cart OR was added on to a new location
	}
	else
	{
		if (found == 0)
		{
			sprintf(str, "Item not added to cart: Not found in database.\n");
			gtk_text_buffer_insert_at_cursor(message, str, -1);
		}
		if (quantity <= 0)
		{
			sprintf(str, "Item not added to cart: Invalid quantity\n");
			gtk_text_buffer_insert_at_cursor(message, str, -1);
		}
	}
	update_cart_view();

	discount_p = atof(gtk_entry_buffer_get_text(discountbuf));
	if (discount_p < 0 && discount_p > 100)
	{
		gtk_entry_buffer_set_text(discountbuf, "0.0", -1);
	}

	GST_p = atof(gtk_entry_buffer_get_text(GSTbuf));
	if (GST_p < 0 && GST_p > 100)
	{
		gtk_entry_buffer_set_text(GSTbuf, "0.0", -1);
	}

	bill_total = total;
	discount_p /= 100.0;
	GST_p /= 100.0;
	discount = bill_total * discount_p;
	bill_total -= discount;
	GST = bill_total * GST_p;
	bill_total += GST;

	sprintf(str, "%.1f", bill_total);
	gtk_text_buffer_set_text(totalbuffer, str, -1);
	tendered = atof(gtk_entry_buffer_get_text(tenderedbuffer));
	tendered_update();
	update_data_buffer();
}

G_MODULE_EXPORT void on_startButton_clicked(GtkButton *uselessvar1)
{
	if (pass_check((char *) gtk_entry_buffer_get_text(quanbuffer), "password"))
	{
		gtk_entry_buffer_set_text(quanbuffer, "", -1);
		gtk_widget_show(ChoiceWindow);
		gtk_widget_hide(startWindow);
		gtk_text_buffer_set_text(message, "", -1);
		update_data_buffer();
		intialize_arr_val(customer_cart, -1, 100);
		update_cart_view();
		backup_stock();
		gtk_text_buffer_set_text(message, "Ready to take orders.", -1);
	}
	else
		gtk_entry_buffer_set_text(quanbuffer, "Incorrect Try Again", -1);	
}

G_MODULE_EXPORT void on_tenderedentry_changed(GtkEntry *ee)
{
	gchar str[36];
	tendered = atof(gtk_entry_buffer_get_text(tenderedbuffer));
	discount_p = atof(gtk_entry_buffer_get_text(discountbuf));
	GST_p = atof(gtk_entry_buffer_get_text(GSTbuf));
	bill_total = total;
	discount_p /= 100.0;
	GST_p /= 100.0;
	discount = bill_total * discount_p;
	bill_total -= discount;
	GST = bill_total * GST_p;
	bill_total += GST;
	change = tendered - bill_total;
	sprintf(str, "Change: %.2f", change);
	gtk_label_set_text(GTK_LABEL(changelabel), str);
}

G_MODULE_EXPORT void on_generate_clicked(GtkButton *uselessvar4)
{

	gtk_text_buffer_set_text(message, "Generating bill", -1);
	gtk_text_buffer_set_text(billbuffer, "\0", -1);
	gchar str[52];
	if (tendered < bill_total)
	{
		sprintf(str, "Insufficient ammount tendered by the customer must be atleast %.1f\n", bill_total);
		gtk_text_buffer_set_text(message, str, -1);
		return;
	}
	
	do
	{
		sprintf(file, "\\Customer Bill %02d.txt", ++customer_no); // Loop looks for existing customer bill files
		strcpy(bildir, cwd);							// If e.g. Bill 07 exists, counter is incremented and we create a file and start from Bill 08
		strcat(bildir, "\\Customer Bills");
		strcat(bildir, file);			// This is a very extra feature...
		bil_fptr = fopen(bildir, "r+"); // it could have just started from 1 each time the program started
	} while (bil_fptr != NULL);

	sprintf(file, "\\Customer Bill %02d.txt", customer_no); // sprintf()
	strcpy(bildir, cwd);
	strcat(bildir, "\\Customer Bills");
	strcat(bildir, file);
	bil_fptr = fopen(bildir, "w+");

	for (unique_items = 0; unique_items < 100 && customer_cart[unique_items][0] != -1; unique_items++)
		;
	fprintf(bil_fptr, " _______________________________________________\n");
	fprintf(bil_fptr, "| |\n");
	fprintf(bil_fptr, "| \\ /\\ / |\\ /| |\n");
	fprintf(bil_fptr, "| \\/ \\/E | \\/ |ART |\n");
	fprintf(bil_fptr, "| Save More. Buy More. |\n");
	fprintf(bil_fptr, "| |\n");
	fprintf(bil_fptr, "| FAST NUCES Karachi Campus, |\n");
	fprintf(bil_fptr, "| Shah Latif Town, NH-5, Karachi, |\n");
	fprintf(bil_fptr, "| Pakistan |\n");
	fprintf(bil_fptr, "| Contact No: +92 (021) 111244622 |\n");
	fprintf(bil_fptr, "| |\n");
	fprintf(bil_fptr, "| Receipt No: %3d |\n", customer_no);
	fprintf(bil_fptr, "| Date: %-18s |\n", time_to_string(timeStr));
	fprintf(bil_fptr, "| Cashier: %-18s |\n", cashierStr);
	fprintf(bil_fptr, "| |\n");
	fprintf(bil_fptr, "|===============================================|\n");
	fprintf(bil_fptr, "| Item Name QTY Price Sub Total |\n");
	fprintf(bil_fptr, "|-----------------------------------------------|\n");

	if (unique_items == 0)
	{
		sprintf(str, "Nothing in customer cart printing empty bill.\n");
		gtk_text_buffer_insert_at_cursor(message, str, -1);
	}

	for (r = 0; r < unique_items; r++)
	{
		address = customer_cart[r][0], quantity = customer_cart[r][1], sub_total = customer_cart[r][2];
		for (name_size = 0; data_array[address].name[name_size] != '\0'; name_size++)
			;
		fprintf(bil_fptr, "| %-12.12s %3d %8.1f %11.1f |\n", data_array[address].name, quantity, data_array[address].price, sub_total);
		if (name_size > 12)
			fprintf(bil_fptr, "| %-12.12s |\n", &data_array[address].name[12]);
		if (name_size > 24)
			fprintf(bil_fptr, "| %-12.12s |\n", &data_array[address].name[24]);
	}

	fprintf(bil_fptr, "|===============================================|\n");
	fprintf(bil_fptr, "| Total: %15.1f |\n", total);
	fprintf(bil_fptr, "| Discount @ %-5.2f: %15.1f |\n", discount_p * 100.0, discount);
	fprintf(bil_fptr, "| GST @ %-5.2f: %15.1f |\n", GST_p * 100.0, GST);
	fprintf(bil_fptr, "|===============================================|\n");
	fprintf(bil_fptr, "| Bill Total: %15.1f |\n", bill_total);
	fprintf(bil_fptr, "| Tendered: %15.1f |\n", tendered);
	fprintf(bil_fptr, "| Change: %15.1f |\n", change);
	fprintf(bil_fptr, "|===============================================|\n");
	fprintf(bil_fptr, "| THANK YOU |\n");
	fprintf(bil_fptr, "|_______________________________________________|");

	fclose(bil_fptr);
	bil_fptr = fopen(bildir, "r");
	do
	{
		str[0] = fgetc(bil_fptr), str[1] = '\0';
		gtk_text_buffer_insert_at_cursor(billbuffer, str, -1);
	} while (!feof(bil_fptr));
	fclose(bil_fptr);

	sprintf(buffer, "\nReceipt stored in %s\nOpening receipt for printing...", bildir);
	gtk_text_buffer_insert_at_cursor(billbuffer, str, -1);
	snprintf(buffer, 300, "\"%s\"", bildir); // Edit this to open with required program e.g chrome etc.
	system(buffer);
	intialize_arr_val(customer_cart, -1, 100);
	total = 0;
	bill_total = 0;
	update_data_buffer();
	update_cart_view();
	array_to_csv(data_array, data_dir, indexVar);
}

G_MODULE_EXPORT void on_quitButton_clicked(GtkButton *bb)
{
	gtk_widget_hide(window);
	gtk_widget_show(ChoiceWindow);
	update_data_buffer();
}

G_MODULE_EXPORT void on_CheckoutButton_clicked(GtkButton *bb)
{
	gtk_widget_hide(ChoiceWindow);
	gtk_widget_show(window);
}

G_MODULE_EXPORT void on_abortButton_clicked(GtkButton *bbb)
{
	intialize_arr_val(customer_cart, -1, 100);
	update_cart_view();
	undo_stock_changes();
	update_data_buffer();
	gtk_entry_buffer_set_text(tenderedbuffer, "0\0", -1);
	gtk_text_buffer_set_text(totalbuffer, "0", -1);
	total = 0;
}

G_MODULE_EXPORT void on_EditButton_clicked(GtkButton *bbs)
{
	gtk_widget_hide(ChoiceWindow);
	gtk_widget_show(EditWindow);
}

G_MODULE_EXPORT void on_quit2_clicked(GtkButton *bbb)
{
	gtk_widget_hide(EditWindow);
	gtk_widget_show(ChoiceWindow);
}

G_MODULE_EXPORT void on_AddButton_clicked(GtkButton *bbA)
{
	gtk_text_buffer_set_text(message, "", -1);
	gchar str[44];
	char name[24];
	int i, j, invalid = 0;
	i = indexVar;
	invalid = 0;
	invalid = atoi(gtk_entry_get_text(GTK_ENTRY(EditNumber)));
	for (j = 0; j < indexVar; j++)
		if (data_array[j].num == invalid || invalid < 0)
		{
			if (invalid < 0)
			{
				sprintf(str, "Invalid item number, please try again\n");
				gtk_text_buffer_insert_at_cursor(message, str, -1);
			}
			else
			{
				sprintf(str, "Item number already exists, please try again\n");
				gtk_text_buffer_insert_at_cursor(message, str, -1);
			}
			return;
		}
		
		strcpy(name, gtk_entry_get_text(GTK_ENTRY(EditItemName)));

		for (j = 0; j < indexVar; j++)
		{
			if (strcmp(name, data_array[j].name) == 0)
			{
				sprintf(str, "Item name already exists, please try again\n");
				gtk_text_buffer_insert_at_cursor(message, str, -1);
				return;
			}
		}
	if (name[0] == '\0') 
	{
		sprintf(str, "Item name cannot be blank\n");
		gtk_text_buffer_insert_at_cursor(message, str, -1);
		return;
	}
	data_array[i].stock = atoi(gtk_entry_get_text(GTK_ENTRY(ItemStock)));
	if (data_array[i].stock == 0) 
	{
		sprintf(str, "Item stock cannot be blank\n");
		gtk_text_buffer_insert_at_cursor(message, str, -1);
		return;
	}
	data_array[i].price = atof(gtk_entry_get_text(GTK_ENTRY(EditPrice)));
	if (data_array[i].price == 0) 
	{
		sprintf(str, "Item price cannot be blank\n");
		gtk_text_buffer_insert_at_cursor(message, str, -1);
		return;
	}
	data_array[i].num = invalid;
	strcpy(data_array[i].name, name);
	indexVar++;
	sprintf(str, "%s, item number: %d, added successfully!\n", data_array[i].name, data_array[i].num);
	gtk_text_buffer_set_text(message, str, -1);
	backup_stock();
	update_data_buffer();
}

G_MODULE_EXPORT void on_RemoveButton_clicked(GtkButton *bbs)
{
	gchar str[40];
	int num, i, found = 0;
	num = atoi(gtk_entry_get_text(GTK_ENTRY(EditNumber)));

	for (i = 0; i < 100 && found == 0; i++)
	{
		if (data_array[i].num == num)
			found = 1;
	}
	i--;
	if (found == 1)
	{
		sprintf(str, "\n%s, item number: %d, removed successfully!\n", data_array[i].name, data_array[i].num);
		data_array[i].num = -1;
		array_to_csv(data_array, data_dir, indexVar);
		indexVar = csv_to_array(data_array, POS_Data_fptr);
	}
	else
		sprintf(str, "Oops! No item of that item number exists.\n");
	gtk_text_buffer_set_text(message, str, -1);
	backup_stock();
	update_data_buffer();
}

G_MODULE_EXPORT void on_UpdateButton_clicked(GtkButton *heu)
{
	gtk_text_buffer_set_text(message, "\0", -1);
	int num, updated = -1, found, i;
	gchar str[128];
	found = 0;
	num = atoi(gtk_entry_get_text(GTK_ENTRY(EditNumber)));
	if (num < 0)
	{
		sprintf(str, "Invalid item number, please try again\n");
		gtk_text_buffer_insert_at_cursor(message, str, -1);
		return;
	}

	for (i = 0; i < indexVar; i++)
		if (num == data_array[i].num)
		{
			found = 1;
			break;
		}

	if (found == 0)
	{
		sprintf(str, "There is no such item with this item number, please try again\n");
		gtk_text_buffer_insert_at_cursor(message, str, -1);
		return;
	}
	else
	{
		sprintf(str, "Updating the stock of %s, item number: %d\n", data_array[i].name, data_array[i].num);
		gtk_text_buffer_insert_at_cursor(message, str, -1);
	}

	sprintf(str, "Enter the updated amount of stock you want for this item: ");
	updated = atoi(gtk_entry_get_text(GTK_ENTRY(ItemStock)));
	if (updated < 0)
	{
		sprintf(str, "Invalid stock requirement\n");
		gtk_text_buffer_insert_at_cursor(message, str, -1);
		return;
	}
	data_array[i].stock = updated;
	sprintf(str, "\nStock updated successfully!");
	gtk_text_buffer_insert_at_cursor(message, str, -1);
	backup_stock();
	update_data_buffer();
}

void update_data_buffer()
{
	int i;
	char tempStr[512];
	gtk_text_buffer_set_text(databasebuffer, "", -1);
	sprintf(tempStr, " _____________________________________________________________________\n");
	gtk_text_buffer_insert_at_cursor(databasebuffer, tempStr, -1);
	sprintf(tempStr, "| Item No | Item Name | Item price | Item Stock |\n");
	gtk_text_buffer_insert_at_cursor(databasebuffer, tempStr, -1);
	sprintf(tempStr, "|==========|=============================|===============|============|\n");
	gtk_text_buffer_insert_at_cursor(databasebuffer, tempStr, -1);
	for (i = 0; i < indexVar; i++) // printing backend stock data
	{
		sprintf(tempStr, "| %-8d | %-27.27s | %13.1f | %10d |\n", data_array[i].num, data_array[i].name, data_array[i].price, data_array[i].stock);
		gtk_text_buffer_insert_at_cursor(databasebuffer, tempStr, -1);
	}
	sprintf(tempStr, "|__________|_____________________________|_______________|____________|\n");
	gtk_text_buffer_insert_at_cursor(databasebuffer, tempStr, -1);
}

void update_cart_view()
{
	int i;
	gchar str[128];
	gtk_text_buffer_set_text(cartbuffer, " Item Quant Sub-total\n", -1);
	
	for (i = 0; i < 100 && customer_cart[i][0] != -1; i++)
	{
		sprintf(str, "| %-25.25s | x%02d | %13.2f |\n", data_array[(int) customer_cart[i][0]].name, (int)customer_cart[i][1], customer_cart[i][2]);
		gtk_text_buffer_insert_at_cursor(cartbuffer, str, -1);
	}
}

void tendered_update()
{
	gchar str[36];
	tendered = atof(gtk_entry_buffer_get_text(tenderedbuffer));
	change = tendered - bill_total;
	sprintf(str, "Change: %.2f", change);
	gtk_label_set_text(GTK_LABEL(changelabel), str);
}
/* After making changes to the array, the array can be stored back in to the .csv file using this function*/
void array_to_csv(item array[], char data_dir[], int indexVar)
{
	int c;
	FILE *ptr;
	ptr = fopen(data_dir, "w+");
	fprintf(ptr, "Item No#,Name,Price,Stock\n");

	for (c = 0; c < indexVar; c++)
		if (array[c].num != -1)
			fprintf(ptr, "%d,%s,%lf,%d\n", array[c].num, array[c].name, array[c].price, array[c].stock);
	fclose(ptr);
}

/*This function Stores values from .CSV file to an array for quick access.
 Returns indexVar of last entered record, meaning total records. Returns -1 incase of an error */
int csv_to_array(item array[], FILE *ptr)
{
	int scanned = 0, indexVar = 0;
	rewind(ptr);
	fscanf(ptr, "Item No#,Name,Price,Stock\n");
	do
	{
		scanned = fscanf(ptr, "%d,%49[^,],%f,%d\n", &array[indexVar].num, array[indexVar].name, &array[indexVar].price, &array[indexVar].stock);
		if (scanned != 4)
			return -1;
		indexVar++;
	} while (!feof(ptr) && scanned == 4);

	return indexVar;
}

/* This function prints time in our required format in the string passed in parameter.
	Returns the first address of the same string. for use in printf("%s", time_to_string(eg_string)); */
char *time_to_string(char *str)
{
	time_t now_time = time(NULL);
	struct tm *l_time = localtime(&now_time);
	strftime(str, 19, "%d-%b-%y %H:%M:%S", l_time);
	return str;
}

/* Sets first column of 2D array to desired value and sets other columns to 0.*/
void intialize_arr_val(float arr[][3], float val, int size)
{
	int x, y, err = 0;
	for (x = 0; x < size; x++)
	{
		arr[x][0] = val;
		arr[x][1] = 0;
		arr[x][2] = 0;
	}
}

int removeFromCart(float cart[][3], item items[100], float *total)
{
	int item_no, quan, i, found = 0, invalid = 0;
	printf("\nEnter item number to remove: ");
	scanf("%d", &item_no);
	for (i = 0; i < 100 && found == 0; i++)
		if (item_no == cart[i][0])
			found = 1;
	if (found == 0)
	{
		printf("\nItem not found.\n");
		return 1;
	}

	printf("quantity to remove: ");
	do
	{
		scanf("%d", &quan);
		if (quan > cart[i][1])
		{
			printf("Invalid entry: entered quantity is greater than cart.\n");
			invalid = 1;
		}
		else
			invalid = 0;		
	} while (invalid = 1);
}

int pass_check(const char *pass, const char *correct_pass)
{
	int len = strlen(correct_pass), i, match = 0;
	if (len == strlen(correct_pass))
		for (i = 0; i < len; i++)
			if (pass[i] == correct_pass[i])
				match++;
	if (match == len)
		return 1;
	else
		return 0;
}

void backup_stock()
{
	int x;
	for (x = 0; x < 200; x++)
		data_array_stock_backup[x] = data_array[x].stock;
	
}

void undo_stock_changes()
{
	int x;
	for (x = 0; x < 200; x++)
		data_array[x].stock = data_array_stock_backup[x];
}