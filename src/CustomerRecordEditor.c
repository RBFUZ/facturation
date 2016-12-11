#include <CustomerRecordEditor.h>

static int CustomerRecord_isValueValid_alwaysAccept(const char * UNUSED(value))
{
  return 1;
}

static int (*alwaysAccept)(const char *) = CustomerRecord_isValueValid_alwaysAccept;

static CustomerRecord_FieldProperties CustomerRecord_FieldsProperties[CUSTOMERRECORD_FIELDCOUNT] =
{
{CUSTOMERRECORD_NAME_SIZE, 200, "Nom", 0, CUSTOMERRECORD_NAME_SIZE - 1, &alwaysAccept, &CustomerRecord_setValue_name, &CustomerRecord_getValue_name},
{CUSTOMERRECORD_ADDRESS_SIZE, 300, "Adresse", 0,
CUSTOMERRECORD_ADDRESS_SIZE - 1, &alwaysAccept, &CustomerRecord_setValue_address, &CustomerRecord_getValue_address},
{
CUSTOMERRECORD_POSTALCODE_SIZE, 100, "Code postal", 0,
CUSTOMERRECORD_POSTALCODE_SIZE - 1, &alwaysAccept, &CustomerRecord_setValue_postalCode, &CustomerRecord_getValue_postalCode},
{
CUSTOMERRECORD_TOWN_SIZE, 200, "Ville", 0, CUSTOMERRECORD_TOWN_SIZE - 1, &alwaysAccept, &CustomerRecord_setValue_town, &CustomerRecord_getValue_town}};

CustomerRecord_FieldProperties CustomerRecord_getFieldProperties(int field)
{
  if (field < 0 || field >= CUSTOMERRECORD_FIELDCOUNT)
    fatalError("CustomerRecord_GetFieldProperties: invalid index");
  return CustomerRecord_FieldsProperties[field];
}

int CustomerRecord_edit(CustomerRecord * record)
{
  GtkWidget *dialog;
  GtkWidget *hbox;
  GtkWidget *table;
  GtkWidget * entries[CUSTOMERRECORD_FIELDCOUNT];
  GtkWidget *label;
  gint response;
  guint i;
  gboolean valid;

  dialog = gtk_dialog_new_with_buttons("Edition d'un client", NULL, GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT, GTK_STOCK_OK, GTK_RESPONSE_OK,
  GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);

  hbox = gtk_hbox_new(FALSE, 8);
  gtk_container_set_border_width(GTK_CONTAINER(hbox), 8);
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox), hbox, FALSE, FALSE, 0);

  table = gtk_table_new(CUSTOMERRECORD_FIELDCOUNT, 2, FALSE);
  gtk_table_set_row_spacings(GTK_TABLE(table), 4);
  gtk_table_set_col_spacings(GTK_TABLE(table), 4);
  gtk_box_pack_start(GTK_BOX(hbox), table, TRUE, TRUE, 0);

  for(i = 0; i < CUSTOMERRECORD_FIELDCOUNT; ++i)
  {
    CustomerRecord_FieldProperties properties = CustomerRecord_getFieldProperties((int) i);
    char * content = (*properties.getValue)(record);

    label = gtk_label_new(properties.name);
    gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, i, i + 1);

    entries[i] = gtk_entry_new();

    gtk_entry_set_text(GTK_ENTRY(entries[i]), content);
    if (properties.maxlength != 0)
      gtk_entry_set_max_length(GTK_ENTRY(entries[i]), properties.maxlength);
    gtk_table_attach_defaults(GTK_TABLE(table), entries[i], 1, 2, i, i + 1);
    gtk_label_set_mnemonic_widget(GTK_LABEL(label), entries[i]);
    free(content);
  }

  gtk_widget_show_all(hbox);
  do
  {
    response = gtk_dialog_run(GTK_DIALOG(dialog));

    valid = (response == GTK_RESPONSE_OK);
    if (valid)
      for(i = 0; i < CUSTOMERRECORD_FIELDCOUNT; ++i)
      {
        CustomerRecord_FieldProperties properties = CustomerRecord_getFieldProperties((int) i);
        if (!(*properties.isValueValid)(gtk_entry_get_text(GTK_ENTRY(entries[i]))))
        {
          GtkWidget *msgdialog;

          msgdialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE,
              "Le contenu du champ \"%s\" est invalide", properties.name);
          g_signal_connect(msgdialog, "response", G_CALLBACK(gtk_widget_destroy), NULL);
          gtk_dialog_run(GTK_DIALOG(msgdialog));
          valid = 0;
          break;
        }
      }

    if (valid)
    {
      for(i = 0; i < CUSTOMERRECORD_FIELDCOUNT; ++i)
      {
        CustomerRecord_FieldProperties properties = CustomerRecord_getFieldProperties((int) i);
        (*properties.setValue)(record, gtk_entry_get_text(GTK_ENTRY(entries[i])));
      }
    }
  }
  while (response == GTK_RESPONSE_OK && !valid);

  gtk_widget_destroy(dialog);

  return (response == GTK_RESPONSE_OK && valid);
}

