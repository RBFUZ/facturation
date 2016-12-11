#include <CatalogRecordEditor.h>

/** Static function which always accepts the value
 * @param value the value to test
 * @return true
 */
static int CatalogRecord_isValueValid_alwaysAccept(const char * UNUSED( value))
{
  return 1;
}

static int (*alwaysAccept)(const char * value) = CatalogRecord_isValueValid_alwaysAccept;

/** Static registry for field descriptions */
static CatalogRecord_FieldProperties CatalogRecord_fieldsProperties[CATALOGRECORD_FIELDCOUNT] =
{
{CATALOGRECORD_CODE_SIZE, 100, "Code", 0, CATALOGRECORD_CODE_SIZE - 1, &CatalogRecord_isValueValid_code, &CatalogRecord_setValue_code,
    &CatalogRecord_getValue_code},
{CATALOGRECORD_DESIGNATION_SIZE, 300, "Désignation", 0, CATALOGRECORD_DESIGNATION_SIZE - 1, &alwaysAccept, &CatalogRecord_setValue_designation,
    &CatalogRecord_getValue_designation},
{CATALOGRECORD_UNITY_SIZE, 100, "Unité", 0.5, CATALOGRECORD_UNITY_SIZE - 1, &alwaysAccept, &CatalogRecord_setValue_unity,
    &CatalogRecord_getValue_unity},
{
CATALOGRECORD_BASEPRICE_SIZE, 100, "Prix de revient", 1, 10, &CatalogRecord_isValueValid_positiveNumber, &CatalogRecord_setValue_basePrice,
    &CatalogRecord_getValue_basePrice},
{CATALOGRECORD_SELLINGPRICE_SIZE, 100, "Prix de vente", 1, 10, &CatalogRecord_isValueValid_positiveNumber, &CatalogRecord_setValue_sellingPrice,
    &CatalogRecord_getValue_sellingPrice},
{
CATALOGRECORD_RATEOFVAT_SIZE, 100, "Taux TVA (en %)", 1, 10, &CatalogRecord_isValueValid_positiveNumber, &CatalogRecord_setValue_rateOfVAT,
    &CatalogRecord_getValue_rateOfVAT}};

/** Get a copy of the properties of a field
 * @param field the identifier of the field
 * @return the properties
 * @see CatalogRecord_FieldIdentifier
 */
CatalogRecord_FieldProperties CatalogRecord_getFieldProperties(int field)
{
  if (field < 0 || field >= CATALOGRECORD_FIELDCOUNT)
    fatalError("CatalogRecord_GetFieldProperties: invalid index");
  return CatalogRecord_fieldsProperties[field];
}

/** CatalogRecordEditor structure
 * @internal
 */
typedef struct _CatalogRecordEditor
{
  GtkWidget * entries[CATALOGRECORD_FIELDCOUNT];
  gulong insert_text_handler[CATALOGRECORD_FIELDCOUNT];
  CatalogRecord * record;
} CatalogRecordEditor;

/** Compute the field id associated with an entry
 * @param catalogRecordEditor a pointer on the CatalogRecordEditor structure
 * @param entry the entry to associate with an id
 * @return the field id
 */
static int CatalogRecordEditor_getEntryOffset(CatalogRecordEditor * catalogRecordEditor, GtkWidget * entry)
{
  int i;
  for(i = 0; i < CATALOGRECORD_FIELDCOUNT; ++i)
    if (catalogRecordEditor->entries[i] == entry)
      return i;
  return -1;
}

/** Static handler to validate input for entries
 * @see insert_text signal of GtkEntry (GTK+)
 */
static void CatalogRecordEditor_insert_text_handler(GtkWidget *entry, const gchar *text, gint length, gint *position,
    CatalogRecordEditor * catalogRecordEditor)
{
  int offset = CatalogRecordEditor_getEntryOffset(catalogRecordEditor, entry);
  CatalogRecord_FieldProperties properties = CatalogRecord_getFieldProperties(offset);
  const char * allText = gtk_entry_get_text(GTK_ENTRY(entry));
  char * result = insertString(allText, *position, text, length);

  GtkEditable *editable = GTK_EDITABLE(entry);

  if ((*properties.isValueValid)(result))
  {
    g_signal_handler_block(G_OBJECT(editable), catalogRecordEditor->insert_text_handler[offset]);
    gtk_editable_insert_text(editable, text, length, position);
    g_signal_handler_unblock(G_OBJECT(editable), catalogRecordEditor->insert_text_handler[offset]);
  }
  g_signal_stop_emission_by_name(G_OBJECT(editable), "insert_text");
  free(result);
}

/** Static handler to format input for entries
 * @see focus-out-event signal of GtkEntry (GTK+)
 */
static gboolean CatalogRecordEditor_focus_out_event(GtkEntry * entry, GdkEventFocus * UNUSED( event), CatalogRecordEditor * catalogRecordEditor)
{
  CatalogRecord record;
  int offset = CatalogRecordEditor_getEntryOffset(catalogRecordEditor, GTK_WIDGET(entry));
  CatalogRecord_FieldProperties properties = CatalogRecord_getFieldProperties(offset);
  CatalogRecord_init(&record);

  (*properties.setValue)(&record, gtk_entry_get_text(entry));
  gtk_entry_set_text(entry, (*properties.getValue)(&record));

  CatalogRecord_finalize(&record);
  return FALSE;
}

/** Edit a record
 * @param record a pointer to the record to edit
 * @return a non null value if the changes were valided, 0 otherwise
 */
int CatalogRecord_edit(CatalogRecord * record)
{
  GtkWidget *dialog;
  GtkWidget *hbox;
  GtkWidget *table;
  GtkWidget *label;
  gint response;
  int i;
  gboolean valid;
  CatalogRecordEditor catalogRecordEditor;

  catalogRecordEditor.record = record;

  dialog = gtk_dialog_new_with_buttons("Edition d'un produit", NULL, GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT, GTK_STOCK_OK, GTK_RESPONSE_OK,
  GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);

  hbox = gtk_hbox_new(FALSE, 8);
  gtk_container_set_border_width(GTK_CONTAINER(hbox), 8);
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox), hbox, FALSE, FALSE, 0);

  table = gtk_table_new(CATALOGRECORD_FIELDCOUNT, 2, FALSE);
  gtk_table_set_row_spacings(GTK_TABLE(table), 4);
  gtk_table_set_col_spacings(GTK_TABLE(table), 4);
  gtk_box_pack_start(GTK_BOX(hbox), table, TRUE, TRUE, 0);

  for(i = 0; i < CATALOGRECORD_FIELDCOUNT; ++i)
  {
    CatalogRecord_FieldProperties properties = CatalogRecord_getFieldProperties((int) i);
    char * content = (*properties.getValue)(record);

    label = gtk_label_new(properties.name);
    gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, (guint) i, (guint) i + 1);

    catalogRecordEditor.entries[i] = gtk_entry_new();

    gtk_entry_set_text(GTK_ENTRY(catalogRecordEditor.entries[i]), content);
    if (properties.maxlength != 0)
      gtk_entry_set_max_length(GTK_ENTRY(catalogRecordEditor.entries[i]), properties.maxlength);
    catalogRecordEditor.insert_text_handler[i] = g_signal_connect(G_OBJECT(catalogRecordEditor.entries[i]), "insert_text",
        G_CALLBACK(CatalogRecordEditor_insert_text_handler), &catalogRecordEditor);
    g_signal_connect(G_OBJECT(catalogRecordEditor.entries[i]), "focus-out-event", G_CALLBACK(CatalogRecordEditor_focus_out_event),
        &catalogRecordEditor);

    gtk_table_attach_defaults(GTK_TABLE(table), catalogRecordEditor.entries[i], 1, 2, (guint) i, (guint) i + 1);
    gtk_label_set_mnemonic_widget(GTK_LABEL(label), catalogRecordEditor.entries[i]);
    free(content);
  }

  gtk_widget_show_all(hbox);
  do
  {
    response = gtk_dialog_run(GTK_DIALOG(dialog));

    valid = (response == GTK_RESPONSE_OK);
    if (valid)
      for(i = 0; i < CATALOGRECORD_FIELDCOUNT; ++i)
      {
        CatalogRecord_FieldProperties properties = CatalogRecord_getFieldProperties(i);
        if (!(*properties.isValueValid)(gtk_entry_get_text(GTK_ENTRY(catalogRecordEditor.entries[i]))))
        {
          GtkWidget * msgdialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE,
              "Le contenu du champ \"%s\" est invalide", properties.name);
          g_signal_connect(msgdialog, "response", G_CALLBACK(gtk_widget_destroy), NULL);
          gtk_dialog_run(GTK_DIALOG(msgdialog));
          valid = 0;
          break;
        }
      }

    if (valid)
    {
      for(i = 0; i < CATALOGRECORD_FIELDCOUNT; ++i)
      {
        CatalogRecord_FieldProperties properties = CatalogRecord_getFieldProperties(i);
        (*properties.setValue)(record, gtk_entry_get_text(GTK_ENTRY(catalogRecordEditor.entries[i])));
      }
    }
  }
  while (response == GTK_RESPONSE_OK && !valid);

  gtk_widget_destroy(dialog);

  return (response == GTK_RESPONSE_OK && valid);
}
