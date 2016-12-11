/*
 * Copyright 2010 Sébastien Aupetit <sebastien.aupetit@univ-tours.fr>
 *
 * This source code is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * This source code is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this source code. If not, see <http://www.gnu.org/licenses/>.
 *
 * $Id: Bill.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <Config.h>
#include <Bill.h>
#include <Operator.h>
#include <DocumentEditor.h>
#include <time.h>
#include <DocumentUtil.h>
#include <Document.h>
#include <DocumentRowList.h>

/** Save a document as a bill
 * @param document the document
 */
void Bill_save(Document * document)
{
  char buf[1024];
  snprintf(buf, 1024, BASEPATH "/data/bill-%s.dat", document->docNumber);
  Document_saveToFile(document, buf);
}

/** Load a document as a bill
 * @param docNumber the document  number
 * @param document the document to fill
 */
void Bill_load(const char * docNumber, Document * document)
{
  char buf[1024];
  snprintf(buf, 1024, BASEPATH "/data/bill-%s.dat", docNumber);
  Document_loadFromFile(document, buf);
}

static GtkTreeModel * Bill_loadModel(void)
{
  GtkListStore * store;
  GtkTreeIter iter;
  GDir * directory;
  Document document;

  Document_init(&document);

  store = gtk_list_store_new(4, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);

  directory = g_dir_open(BASEPATH "/data", 0, NULL);
  if (directory == NULL)
  {
    GtkWidget *errordialog;
    errordialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE,
        "Impossible de lister les factures");
    g_signal_connect(errordialog, "response", G_CALLBACK (gtk_widget_destroy), NULL);
    gtk_widget_show(errordialog);
  }
  else
  {
    const gchar * entry;
    while ((entry = g_dir_read_name(directory)) != NULL)
    {
      if (icaseStartWith("bill-", entry) && icaseEndWith(".dat", entry))
      {
        char * filename = concatenateString(BASEPATH "/data/", entry);
        Document_loadFromFile(&document, filename);
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter, 0, document.docNumber, 1, document.customer.name, 2, document.editDate, 3, document.object, -1);
        free(filename);
      }
    }
    g_dir_close(directory);
  }

  Document_finalize(&document);
  return GTK_TREE_MODEL(store);
}

static int Bill_select(Document * document)
{
  GtkWidget *dialog;
  int result = 0;
  GtkWidget * sw;
  GtkTreeModel * model, *sortedmodel;
  GtkWidget *treeview;
  GtkCellRenderer * renderer;
  GtkTreeViewColumn *column;

  dialog = gtk_dialog_new_with_buttons("Choix d'une facture", NULL, GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT, GTK_STOCK_OK, GTK_RESPONSE_OK,
      GTK_STOCK_CLOSE, GTK_RESPONSE_CLOSE, NULL);

  gtk_container_set_border_width(GTK_CONTAINER (dialog), 5);

  sw = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW (sw), GTK_SHADOW_ETCHED_IN);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW (sw), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_box_pack_start(GTK_BOX (GTK_DIALOG (dialog)->vbox), sw, TRUE, TRUE, 0);

  model = Bill_loadModel();
  sortedmodel = gtk_tree_model_sort_new_with_model(model);

  /* create tree view */
  treeview = gtk_tree_view_new_with_model(sortedmodel);
  gtk_tree_view_set_rules_hint(GTK_TREE_VIEW (treeview), TRUE);
  gtk_tree_view_set_search_column(GTK_TREE_VIEW (treeview), 0);

  renderer = gtk_cell_renderer_text_new();

  column = gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(column, "Numéro");
  gtk_tree_view_column_pack_start(column, renderer, TRUE);
  gtk_tree_view_column_add_attribute(column, renderer, "text", 0);
  gtk_tree_view_column_set_resizable(column, TRUE);
  gtk_tree_view_column_set_min_width(column, 100);
  gtk_tree_view_column_set_sort_column_id(column, 0);
  gtk_tree_view_column_set_alignment(column, 0.5);
  gtk_cell_renderer_set_alignment(renderer, 0, 0.5);
  gtk_tree_view_append_column(GTK_TREE_VIEW (treeview), column);

  column = gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(column, "Nom client");
  gtk_tree_view_column_pack_start(column, renderer, TRUE);
  gtk_tree_view_column_add_attribute(column, renderer, "text", 1);
  gtk_tree_view_column_set_resizable(column, TRUE);
  gtk_tree_view_column_set_min_width(column, 100);
  gtk_tree_view_column_set_max_width(column, 300);
  gtk_tree_view_column_set_sort_column_id(column, 1);
  gtk_tree_view_column_set_alignment(column, 0.5);
  gtk_cell_renderer_set_alignment(renderer, 0, 0.5);
  gtk_tree_view_append_column(GTK_TREE_VIEW (treeview), column);

  column = gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(column, "Date d'édition");
  gtk_tree_view_column_pack_start(column, renderer, TRUE);
  gtk_tree_view_column_add_attribute(column, renderer, "text", 2);
  gtk_tree_view_column_set_resizable(column, TRUE);
  gtk_tree_view_column_set_min_width(column, 100);
  gtk_tree_view_column_set_sort_column_id(column, 2);
  gtk_tree_view_column_set_alignment(column, 0.5);
  gtk_cell_renderer_set_alignment(renderer, 0, 0.5);
  gtk_tree_view_append_column(GTK_TREE_VIEW (treeview), column);

  column = gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(column, "Objet");
  gtk_tree_view_column_pack_start(column, renderer, TRUE);
  gtk_tree_view_column_add_attribute(column, renderer, "text", 3);
  gtk_tree_view_column_set_resizable(column, TRUE);
  gtk_tree_view_column_set_min_width(column, 100);
  gtk_tree_view_column_set_max_width(column, 300);
  gtk_tree_view_column_set_sort_column_id(column, 3);
  gtk_tree_view_column_set_alignment(column, 0.5);
  gtk_cell_renderer_set_alignment(renderer, 0, 0.5);
  gtk_tree_view_append_column(GTK_TREE_VIEW (treeview), column);

  gtk_widget_set_size_request(sw, 800, 400);
  gtk_widget_show(treeview);

  g_object_unref(sortedmodel);
  g_object_unref(model);

  gtk_container_add(GTK_CONTAINER (sw), treeview);
  gtk_widget_show_all(GTK_DIALOG (dialog)->vbox);

  if (gtk_dialog_run(GTK_DIALOG (dialog)) == GTK_RESPONSE_OK)
  {
    GtkTreeSelection * selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
    GtkTreeIter iter, childIter;

    if (gtk_tree_selection_get_selected(selection, NULL, &iter))
    {
      gchar * docNumberGTK;
      gtk_tree_model_sort_convert_iter_to_child_iter(GTK_TREE_MODEL_SORT (sortedmodel), &childIter, &iter);
      gtk_tree_model_get(model, &childIter, 0, &docNumberGTK, -1);
      Bill_load(docNumberGTK, document);
      g_free(docNumberGTK);
      result = 1;
    }
  }
  gtk_widget_destroy(dialog);

  return result;
}

void Bill_new(GtkWidget * UNUSED( widget), GtkWindow * parent)
{
  Document document;
  time_t curTime;
  struct tm * tm;
  char * operator;

  operator = Operator_identify(parent);
  if (operator == NULL)
    return;

  time(&curTime);
  tm = localtime(&curTime);

  Document_init(&document);
  free(document.operator);
  document.operator = operator;
  free(document.docNumber);
  document.docNumber = computeDocumentNumber(curTime);
  free(document.editDate);
  document.editDate = formatDate(tm->tm_mday, tm->tm_mon+1, 1900 + tm->tm_year);
  free(document.expiryDate);
  document.expiryDate = duplicateString("6 mois");

  document.rows = DocumentRow_create();
  document.typeDocument = BILL;
  if (DocumentEditor_edit(parent, NEW_DOCUMENT, &document))
    Bill_save(&document);

  Document_finalize(&document);
}

void Bill_open(GtkWidget * UNUSED(widget), GtkWindow * parent)
{
  Document document;
  char * operator;
  time_t curTime;
  struct tm * tm;

  operator = Operator_identify(parent);
  if (operator == NULL)
    return;

  time(&curTime);
  tm = localtime(&curTime);

  Document_init(&document);
  if (Bill_select(&document))
  {
    free(document.operator);
    document.operator = operator;
    free(document.editDate);
    document.editDate = formatDate(tm->tm_mday, tm->tm_mon+1, 1900 + tm->tm_year);

    if (DocumentEditor_edit(parent, MODIFY_DOCUMENT, &document))
      Bill_save(&document);
  }
  else
    free(operator);
  Document_finalize(&document);
}

void Bill_display(GtkWidget * UNUSED(widget), GtkWindow * parent)
{
  Document document;

  char * operator = Operator_identify(parent);
  if (operator == NULL)
    return;

  Document_init(&document);
  if (Bill_select(&document))
  {
    DocumentEditor_edit(parent, DISPLAY_DOCUMENT, &document);
  }
  else
    free(operator);
  Document_finalize(&document);
}

