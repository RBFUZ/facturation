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
 * $Id: DocumentEditor.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <DocumentEditor.h>
#include <CustomerDB.h>
#include <Customer.h>
#include <gdk/gdkkeysyms.h>
#include <CatalogRecord.h>
#include <CatalogRecordEditor.h>
#include <Catalog.h>
#include <CatalogDB.h>
#include <Print.h>
#include <DocumentRowList.h>

#define EDITOR_ROWCOUNT 4

typedef struct _DocumentEditor {
    Document * document;
    GtkWidget * customerViewer;
    GtkWidget * editDateEntry;
    GtkWidget * expiryDateEntry;
    GtkWidget * docNumberEntry;
    GtkWidget * objectEntry;
    GtkWidget * operatorEntry;
    GtkWidget * sumWithoutVATEntry;
    GtkWidget * sumOfVATEntry;
    GtkWidget * sumWithVATEntry;
    GtkWidget * codeEntry[EDITOR_ROWCOUNT];
    GtkWidget * catalogButton[EDITOR_ROWCOUNT];
    GtkWidget * designationEntry[EDITOR_ROWCOUNT];
    GtkWidget * quantityEntry[EDITOR_ROWCOUNT];
    GtkWidget * unityEntry[EDITOR_ROWCOUNT];
    GtkWidget * basePriceEntry[EDITOR_ROWCOUNT];
    GtkWidget * sellingPriceEntry[EDITOR_ROWCOUNT];
    GtkWidget * discountEntry[EDITOR_ROWCOUNT];
    GtkWidget * finalPriceEntry[EDITOR_ROWCOUNT];
    GtkWidget * rateOfVATEntry[EDITOR_ROWCOUNT];
    GtkWidget * insertBeforeButton[EDITOR_ROWCOUNT];
    GtkWidget * insertAfterButton[EDITOR_ROWCOUNT];
    GtkWidget * removeButton[EDITOR_ROWCOUNT];
    GtkWidget * vscrollbar;

    gulong quantity_insert_text_handler[EDITOR_ROWCOUNT];
    gulong basePrice_insert_text_handler[EDITOR_ROWCOUNT];
    gulong sellingPrice_insert_text_handler[EDITOR_ROWCOUNT];
    gulong discount_insert_text_handler[EDITOR_ROWCOUNT];
    gulong rateOfVAT_insert_text_handler[EDITOR_ROWCOUNT];

} DocumentEditor;

static int isPositiveNumber(const char * value) {
    char * end = NULL;
    double val = strtod(value, &end);
    return (end != value && *end == '\0' && val >= 0);
}

static void DocumentEditor_setCustomer(CustomerRecord * record, GtkWidget * customerViewer) {
    GtkTextIter iter;
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW (customerViewer));

    gtk_text_buffer_set_text(buffer, "", -1);

    gtk_text_buffer_get_iter_at_offset(buffer, &iter, -1);
    gtk_text_buffer_insert(buffer, &iter, record->name, -1);
    gtk_text_buffer_insert(buffer, &iter, "\n", -1);
    gtk_text_buffer_insert(buffer, &iter, record->address, -1);
    gtk_text_buffer_insert(buffer, &iter, "\n", -1);
    gtk_text_buffer_insert(buffer, &iter, record->postalCode, -1);
    gtk_text_buffer_insert(buffer, &iter, " ", -1);
    gtk_text_buffer_insert(buffer, &iter, record->town, -1);
}

static void DocumentEditor_chooseCustomer(GtkWidget * UNUSED( button),
        DocumentEditor * documentEditor) {
    int choix = Customer_select(NULL);

    if (choix != -1) {
        CustomerDB * customerDB = CustomerDB_open(CUSTOMERDB_FILENAME);
        if (customerDB == NULL)
            fatalError("DocumentEditor_ChooseCustomer");

        CustomerDB_readRecord(customerDB, choix, &documentEditor->document->customer);
        DocumentEditor_setCustomer(&documentEditor->document->customer,
                documentEditor->customerViewer);
        CustomerDB_close(customerDB);
    }
}

static int DocumentEditor_getEntryOffset(DocumentEditor * documentEditor, GtkWidget * entry) {
    int i;
    for (i = 0; i < EDITOR_ROWCOUNT; ++i) {
        if (entry == documentEditor->codeEntry[i] || entry == documentEditor->designationEntry[i]
                || entry == documentEditor->unityEntry[i] || entry
                == documentEditor->quantityEntry[i] || entry == documentEditor->basePriceEntry[i]
                || entry == documentEditor->sellingPriceEntry[i] || entry
                == documentEditor->discountEntry[i] || entry == documentEditor->finalPriceEntry[i]
                || entry == documentEditor->rateOfVATEntry[i] || entry
                == documentEditor->catalogButton[i] || entry
                == documentEditor->insertBeforeButton[i] || entry
                == documentEditor->insertAfterButton[i] || entry == documentEditor->removeButton[i])
            return i;
    }
    return -1;
}

static void DocumentEditor_saveData(DocumentEditor * documentEditor, int first) {
    int i;
    Document * document = documentEditor->document;
    /* Phase 1 */
    free(document->expiryDate);
    free(document->object);
    document->expiryDate = duplicateString(gtk_entry_get_text(
            GTK_ENTRY(documentEditor->expiryDateEntry)));
    document->object = duplicateString(gtk_entry_get_text(GTK_ENTRY(documentEditor->objectEntry)));

    /* Phase 2 : les lignes */
    for (i = 0; i < EDITOR_ROWCOUNT; ++i) {
        DocumentRow * row = DocumentRowList_get(document->rows, first + i);
        if (row != NULL) {
            free(row->code);
            row->code = duplicateString(
                    gtk_entry_get_text(GTK_ENTRY (documentEditor->codeEntry[i])));
            free(row->designation);
            row->designation = duplicateString(gtk_entry_get_text(
                    GTK_ENTRY (documentEditor->designationEntry[i])));
            free(row->unity);
            row->unity = duplicateString(gtk_entry_get_text(
                    GTK_ENTRY (documentEditor->unityEntry[i])));
            row->quantity = atof(gtk_entry_get_text(GTK_ENTRY (documentEditor->quantityEntry[i])));
            row->basePrice
                    = atof(gtk_entry_get_text(GTK_ENTRY (documentEditor->basePriceEntry[i])));
            row->sellingPrice = atof(gtk_entry_get_text(
                    GTK_ENTRY (documentEditor->sellingPriceEntry[i])));
            row->discount = atof(gtk_entry_get_text(GTK_ENTRY (documentEditor->discountEntry[i])));
            row->rateOfVAT
                    = atof(gtk_entry_get_text(GTK_ENTRY (documentEditor->rateOfVATEntry[i])));
        }
    }

}

static void DocumentEditor_loadData(DocumentEditor * documentEditor, int first) {
    int i;
    double sumWithoutVAT = 0;
    double sumOfVAT = 0;
    double sumWithVAT = 0;
    double ht;
    char buf[1024];
    int rowCount;
    Document * document = documentEditor->document;
    DocumentRow * cur;

    /* Phase 1 : entete */
    DocumentEditor_setCustomer(&document->customer, documentEditor->customerViewer);
    gtk_entry_set_text(GTK_ENTRY (documentEditor->operatorEntry), document->operator);
    gtk_entry_set_text(GTK_ENTRY (documentEditor->editDateEntry), document->editDate);
    gtk_entry_set_text(GTK_ENTRY (documentEditor->expiryDateEntry), document->expiryDate);
    gtk_entry_set_text(GTK_ENTRY (documentEditor->docNumberEntry), document->docNumber);
    gtk_entry_set_text(GTK_ENTRY (documentEditor->objectEntry), document->object);

    /* Phase 2 : les lignes */
    for (i = 0; i < EDITOR_ROWCOUNT; ++i) {
        DocumentRow * row = DocumentRowList_get(document->rows, first + i);
        gtk_widget_set_sensitive(documentEditor->codeEntry[i], row != NULL);
        gtk_widget_set_sensitive(documentEditor->designationEntry[i], row != NULL);
        gtk_widget_set_sensitive(documentEditor->unityEntry[i], row != NULL);
        gtk_widget_set_sensitive(documentEditor->quantityEntry[i], row != NULL);
        gtk_widget_set_sensitive(documentEditor->basePriceEntry[i], row != NULL);
        gtk_widget_set_sensitive(documentEditor->sellingPriceEntry[i], row != NULL);
        gtk_widget_set_sensitive(documentEditor->discountEntry[i], row != NULL);
        gtk_widget_set_sensitive(documentEditor->finalPriceEntry[i], row != NULL);
        gtk_widget_set_sensitive(documentEditor->rateOfVATEntry[i], row != NULL);
        if (documentEditor->catalogButton[i] != NULL)
            gtk_widget_set_sensitive(documentEditor->catalogButton[i], row != NULL);

        if (row == NULL) {
            gtk_entry_set_text(GTK_ENTRY (documentEditor->codeEntry[i]), "");
            gtk_entry_set_text(GTK_ENTRY (documentEditor->designationEntry[i]), "");
            gtk_entry_set_text(GTK_ENTRY (documentEditor->unityEntry[i]), "");
            gtk_entry_set_text(GTK_ENTRY (documentEditor->quantityEntry[i]), "");
            gtk_entry_set_text(GTK_ENTRY (documentEditor->basePriceEntry[i]), "");
            gtk_entry_set_text(GTK_ENTRY (documentEditor->sellingPriceEntry[i]), "");
            gtk_entry_set_text(GTK_ENTRY (documentEditor->discountEntry[i]), "");
            gtk_entry_set_text(GTK_ENTRY (documentEditor->finalPriceEntry[i]), "");
            gtk_entry_set_text(GTK_ENTRY (documentEditor->rateOfVATEntry[i]), "");

        } else {
            GdkColor color;

            gtk_entry_set_text(GTK_ENTRY (documentEditor->codeEntry[i]), row->code);
            gtk_entry_set_text(GTK_ENTRY (documentEditor->designationEntry[i]), row->designation);
            gtk_entry_set_text(GTK_ENTRY (documentEditor->unityEntry[i]), row->unity);
            snprintf(buf, 1024, "%.2f", (double) (row->quantity));
            gtk_entry_set_text(GTK_ENTRY (documentEditor->quantityEntry[i]), buf);
            gtk_entry_set_editable(GTK_ENTRY(documentEditor->basePriceEntry[i]), strcmp(row->code,
                    "") == 0);
            snprintf(buf, 1024, "%.2f", (double) (row->basePrice));
            gtk_entry_set_text(GTK_ENTRY (documentEditor->basePriceEntry[i]), buf);
            snprintf(buf, 1024, "%.2f", (double) (row->sellingPrice));
            gtk_entry_set_text(GTK_ENTRY (documentEditor->sellingPriceEntry[i]), buf);
            snprintf(buf, 1024, "%.2f", (double) (row->discount));
            gtk_entry_set_text(GTK_ENTRY (documentEditor->discountEntry[i]), buf);
            snprintf(buf, 1024, "%.2f", (double) (row->sellingPrice - row->discount));
            gtk_entry_set_text(GTK_ENTRY (documentEditor->finalPriceEntry[i]), buf);
            snprintf(buf, 1024, "%.2f", (double) (row->rateOfVAT));
            gtk_entry_set_text(GTK_ENTRY (documentEditor->rateOfVATEntry[i]), buf);

            if (row->sellingPrice - row->discount < 0 || row->basePrice > row->sellingPrice || row->basePrice > (row->sellingPrice - row->discount))
                gdk_color_parse("red", &color);
            else
                gdk_color_parse("black", &color);

            gtk_widget_modify_text(documentEditor->sellingPriceEntry[i], GTK_STATE_NORMAL, &color);
            gtk_widget_modify_text(documentEditor->discountEntry[i], GTK_STATE_NORMAL, &color);
            gtk_widget_modify_text(documentEditor->finalPriceEntry[i], GTK_STATE_NORMAL, &color);
        }
    }

    rowCount = DocumentRowList_getRowCount(document->rows);
    if (rowCount > 0) {
        gtk_range_set_range(GTK_RANGE(documentEditor->vscrollbar), 0, rowCount);
        gtk_widget_set_sensitive(documentEditor->vscrollbar, TRUE);
    } else
        gtk_widget_set_sensitive(documentEditor->vscrollbar, FALSE);

    /* Phase 3 : les totaux */
    cur = document->rows;
    while (cur != NULL) {
        ht = (cur->sellingPrice - cur->discount) * cur->quantity;
        sumWithoutVAT += ht;
        sumOfVAT += ht * cur->rateOfVAT / 100.;
        sumWithVAT += ht * (1. + cur->rateOfVAT / 100.);
        cur = cur->next;
    }

    snprintf(buf, 1024, "%.2f", (double) sumWithoutVAT);
    gtk_entry_set_text(GTK_ENTRY (documentEditor->sumWithoutVATEntry), buf);
    snprintf(buf, 1024, "%.2f", (double) sumOfVAT);
    gtk_entry_set_text(GTK_ENTRY (documentEditor->sumOfVATEntry), buf);
    snprintf(buf, 1024, "%.2f", (double) sumWithVAT);
    gtk_entry_set_text(GTK_ENTRY (documentEditor->sumWithVATEntry), buf);
}

static void DocumentEditor_insert_text_handler_positiveNumeric(GtkWidget *entry, const gchar *text,
        gint length, gint *position, DocumentEditor * documentEditor) {
    const char * allText = gtk_entry_get_text(GTK_ENTRY(entry));
    char * result = insertString(allText, *position, text, length);
    int offset = DocumentEditor_getEntryOffset(documentEditor, entry);

    GtkEditable *editable = GTK_EDITABLE(entry);

    if (isPositiveNumber(result)) {
        gulong handler = 0;
        if (entry == documentEditor->basePriceEntry[offset])
            handler = documentEditor->basePrice_insert_text_handler[offset];
        else if (entry == documentEditor->discountEntry[offset])
            handler = documentEditor->discount_insert_text_handler[offset];
        else if (entry == documentEditor->quantityEntry[offset])
            handler = documentEditor->quantity_insert_text_handler[offset];
        else if (entry == documentEditor->rateOfVATEntry[offset])
            handler = documentEditor->rateOfVAT_insert_text_handler[offset];
        else if (entry == documentEditor->sellingPriceEntry[offset])
            handler = documentEditor->sellingPrice_insert_text_handler[offset];

        g_signal_handler_block(G_OBJECT (editable), handler);
        gtk_editable_insert_text(editable, text, length, position);
        g_signal_handler_unblock(G_OBJECT (editable), handler);
    }
    g_signal_stop_emission_by_name(G_OBJECT (editable), "insert_text");
    free(result);
}

gboolean DocumentEditor_handleFocusOut(GtkWidget * UNUSED( widget), GdkEventFocus * UNUSED( event),
        DocumentEditor * documentEditor) {
    int first = (int) gtk_range_get_value(GTK_RANGE(documentEditor->vscrollbar));

    if (first != -1) {
        DocumentEditor_saveData(documentEditor, first);
        DocumentEditor_loadData(documentEditor, first);
    } else
        fprintf(stderr, "DocumentEditor_HandleFocusOut must not be called here\n");
    return FALSE;
}

static gboolean DocumentEditor_handleFirstEntry(GtkWidget *entry, GdkEventKey * event,
        DocumentEditor * documentEditor) {

    if (event->keyval == GDK_ISO_Left_Tab) {
        int first = (int) gtk_range_get_value(GTK_RANGE(documentEditor->vscrollbar));
        int offset = DocumentEditor_getEntryOffset(documentEditor, entry);
        if (offset == 0) {
            if (first > 0) {
                DocumentEditor_saveData(documentEditor, first);
                first--;
                DocumentEditor_loadData(documentEditor, first);
                gtk_range_set_value(GTK_RANGE(documentEditor->vscrollbar), first);
                gtk_widget_grab_focus(documentEditor->rateOfVATEntry[0]);
            }
            return TRUE; /* Stop other handler */
        }
    }
    return FALSE;
}

static gboolean DocumentEditor_handleLastEntry(GtkWidget *entry, GdkEventKey * event,
        DocumentEditor * documentEditor) {

    if (event->keyval == GDK_Tab || event->keyval == GDK_Return) {
        int first = (int) gtk_range_get_value(GTK_RANGE(documentEditor->vscrollbar));
        int offset = DocumentEditor_getEntryOffset(documentEditor, entry);
        if (offset != -1) {
            if (first + offset == DocumentRowList_getRowCount(documentEditor->document->rows) - 1) {
                DocumentEditor_saveData(documentEditor, first);
                DocumentRowList_pushBack(&documentEditor->document->rows, DocumentRow_create());
                offset++;
                if (offset >= EDITOR_ROWCOUNT) {
                    offset = EDITOR_ROWCOUNT - 1;
                    first++;
                }
                DocumentEditor_loadData(documentEditor, first);
                gtk_range_set_value(GTK_RANGE(documentEditor->vscrollbar), first);
                gtk_widget_grab_focus(documentEditor->codeEntry[offset]);
                return TRUE; /* Stop other handler */
            }
        }
    }
    return FALSE;
}

static void DocumentEditor_scrollbarChanged(GtkWidget * UNUSED( scrollbar),
        DocumentEditor * documentEditor) {
    int first = (int) gtk_range_get_value(GTK_RANGE(documentEditor->vscrollbar));
    if (first >= 0) {
        DocumentEditor_loadData(documentEditor, first);
    }
}

static gboolean DocumentEditor_scrollbarBeforeChange(GtkRange * UNUSED( range),
        GtkScrollType UNUSED( scroll), gdouble UNUSED( value), DocumentEditor * documentEditor) {
    int firstBefore = (int) gtk_range_get_value(GTK_RANGE(documentEditor->vscrollbar));

    if (firstBefore >= 0) {
        DocumentEditor_saveData(documentEditor, firstBefore);
    }
    return FALSE;
}

static void DocumentEditor_insertBefore(GtkWidget * button, DocumentEditor * documentEditor) {
    Document * document = documentEditor->document;
    int first = (int) gtk_range_get_value(GTK_RANGE(documentEditor->vscrollbar));
    int offset = DocumentEditor_getEntryOffset(documentEditor, button);

    DocumentRow * row = DocumentRowList_get(document->rows, first + offset);
    if (row != NULL) {
        DocumentRowList_insertBefore(&document->rows, row, DocumentRow_create());
        if (offset == 0) {
            if (first > 0)
                first--;
            DocumentEditor_loadData(documentEditor, first);
            gtk_range_set_value(GTK_RANGE(documentEditor->vscrollbar), first);
            gtk_widget_grab_focus(documentEditor->codeEntry[offset]);
        } else {
            offset--;
            DocumentEditor_loadData(documentEditor, first);
            gtk_widget_grab_focus(documentEditor->codeEntry[offset]);
        }
    }
}

static void DocumentEditor_insertAfter(GtkWidget * button, DocumentEditor * documentEditor) {
    Document * document = documentEditor->document;
    int first = (int) gtk_range_get_value(GTK_RANGE(documentEditor->vscrollbar));
    int offset = DocumentEditor_getEntryOffset(documentEditor, button);

    DocumentRow * row = DocumentRowList_get(document->rows, first + offset);
    if (row != NULL) {
        DocumentRowList_insertAfter(&document->rows, row, DocumentRow_create());
        if (offset == EDITOR_ROWCOUNT - 1) {
            first++;
            DocumentEditor_loadData(documentEditor, first);
            gtk_range_set_value(GTK_RANGE(documentEditor->vscrollbar), first);
            gtk_widget_grab_focus(documentEditor->codeEntry[offset]);
        } else {
            offset++;
            DocumentEditor_loadData(documentEditor, first);
            gtk_widget_grab_focus(documentEditor->codeEntry[offset]);
        }
    }
}

static void DocumentEditor_removeCurrent(GtkWidget * button, DocumentEditor * documentEditor) {
    Document * document = documentEditor->document;
    int first = (int) gtk_range_get_value(GTK_RANGE(documentEditor->vscrollbar));
    int offset = DocumentEditor_getEntryOffset(documentEditor, button);

    DocumentRow * row = DocumentRowList_get(document->rows, first + offset);
    if (row != NULL) {
        int count = DocumentRowList_getRowCount(document->rows);
        if (count > 1) {
            DocumentRowList_removeRow(&document->rows, row);
            if (first > 0 && first < count - EDITOR_ROWCOUNT) {
                first = DocumentRowList_getRowCount(document->rows) - EDITOR_ROWCOUNT;
            }
            if (first + offset >= count)
                offset = count - first - 1;
            DocumentEditor_loadData(documentEditor, first);
            gtk_range_set_value(GTK_RANGE(documentEditor->vscrollbar), first);
            gtk_widget_grab_focus(documentEditor->codeEntry[offset]);
        } else {
            free(row->code);
            row->code = duplicateString("");
            free(row->designation);
            row->designation = duplicateString("");
            free(row->unity);
            row->unity = duplicateString("");
            row->basePrice = 0;
            row->sellingPrice = 0;
            row->rateOfVAT = 0;
            DocumentEditor_loadData(documentEditor, first);
            gtk_range_set_value(GTK_RANGE(documentEditor->vscrollbar), first);
            gtk_widget_grab_focus(documentEditor->codeEntry[offset]);
        }
    }
}

static void DocumentEditor_chooseProduct(GtkWidget * button, DocumentEditor * documentEditor) {
    CatalogDB * catalogDB;
    Document * document = documentEditor->document;
    int first = (int) gtk_range_get_value(GTK_RANGE(documentEditor->vscrollbar));
    int offset = DocumentEditor_getEntryOffset(documentEditor, button);
    DocumentRow * row = DocumentRowList_get(document->rows, first + offset);
    int recordNum = Catalog_select(NULL);

    if (recordNum != -1 && row != NULL) {
        CatalogRecord record;
        CatalogRecord_init(&record);
        catalogDB = CatalogDB_openOrCreate(CATALOGDB_FILENAME);
        CatalogDB_readRecord(catalogDB, recordNum, &record);
        free(row->code);
        row->code = duplicateString(record.code);
        free(row->designation);
        row->designation = duplicateString(record.designation);
        free(row->unity);
        row->unity = duplicateString(record.unity);
        row->basePrice = record.basePrice;
        row->sellingPrice = record.sellingPrice;
        row->rateOfVAT = record.rateOfVAT;
        CatalogDB_close(catalogDB);
        CatalogRecord_finalize(&record);
        DocumentEditor_loadData(documentEditor, first);
        gtk_widget_grab_focus(documentEditor->codeEntry[offset]);
    }
}

int DocumentEditor_edit(GtkWindow * parent, TypeAction typeAction, Document * document) {
    DocumentEditor documentEditor;
    GtkWidget * dialog;
    gint response;
    GtkWidget * button, *hbox, *vbox, *table, *label, *sw, *alignment, *frame;
    guint i;
    GdkColor color;
    const char * title;
    GtkObject * adjustment;
    int first;

    documentEditor.document = document;

    if (typeAction == NEW_DOCUMENT) {
        if (document->typeDocument == QUOTATION)
            title = "Nouveau devis";
        else
            title = "Nouvelle facture";
    } else if (typeAction == DISPLAY_DOCUMENT) {
        if (document->typeDocument == QUOTATION)
            title = "Visualisation d'un devis";
        else
            title = "Visualisationd d'une facture";
    } else {
        if (document->typeDocument == QUOTATION)
            title = "Edition d'un devis";
        else
            title = "Edition d'une facture";
    }

    if (typeAction != DISPLAY_DOCUMENT) {
        dialog = gtk_dialog_new_with_buttons(title, parent, GTK_DIALOG_MODAL
                | GTK_DIALOG_DESTROY_WITH_PARENT, GTK_STOCK_PRINT, 1, GTK_STOCK_SAVE,
                GTK_RESPONSE_OK, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
    } else {
        dialog = gtk_dialog_new_with_buttons(title, parent, GTK_DIALOG_MODAL
                | GTK_DIALOG_DESTROY_WITH_PARENT, GTK_STOCK_PRINT, 1, GTK_STOCK_CLOSE,
                GTK_RESPONSE_CANCEL, NULL);
    }

    gtk_container_set_border_width(GTK_CONTAINER(dialog), 5);

    gtk_container_set_border_width(GTK_CONTAINER(GTK_DIALOG (dialog)->vbox), 5);

    /* customer + choix customer + data + n° */
    hbox = gtk_hbox_new(FALSE, 5);
    gtk_box_pack_start(GTK_BOX (GTK_DIALOG (dialog)->vbox), hbox, TRUE, TRUE, 10);

    vbox = gtk_vbox_new(FALSE, 5);
    gtk_box_pack_start(GTK_BOX (hbox), vbox, TRUE, TRUE, 0);

    if (typeAction != DISPLAY_DOCUMENT) {
        button = gtk_button_new_with_label("Choisir un customer");
        g_signal_connect(button, "clicked", G_CALLBACK(DocumentEditor_chooseCustomer),&documentEditor);
        gtk_box_pack_start(GTK_BOX (vbox), button, TRUE, TRUE, 0);
    }

    documentEditor.customerViewer = gtk_text_view_new();
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW (documentEditor.customerViewer), GTK_WRAP_WORD);
    gtk_text_view_set_editable(GTK_TEXT_VIEW (documentEditor.customerViewer), FALSE);
    gtk_widget_set_size_request(documentEditor.customerViewer, 300, -1);
    gtk_widget_set_can_focus(documentEditor.customerViewer, FALSE);

    sw = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW (sw), GTK_POLICY_AUTOMATIC,
            GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER (sw), documentEditor.customerViewer);
    gtk_box_pack_start(GTK_BOX (vbox), sw, TRUE, TRUE, 0);

    table = gtk_table_new(4, 2, FALSE);
    gtk_table_set_row_spacings(GTK_TABLE (table), 4);
    gtk_table_set_col_spacings(GTK_TABLE (table), 4);

    alignment = gtk_alignment_new(1, 0, 0, 0);
    gtk_container_add(GTK_CONTAINER(alignment), table);
    gtk_box_pack_start(GTK_BOX (hbox), alignment, FALSE, FALSE, 0);

    label = gtk_label_new("Opérateur :");
    alignment = gtk_alignment_new(1, 0, 0, 0);
    gtk_container_add(GTK_CONTAINER(alignment), label);
    gtk_table_attach_defaults(GTK_TABLE (table), alignment, 0, 1, 0, 1);
    documentEditor.operatorEntry = gtk_entry_new();
    gtk_entry_set_editable(GTK_ENTRY (documentEditor.operatorEntry), FALSE);
    gtk_widget_set_can_focus(documentEditor.operatorEntry, FALSE);
    gtk_entry_set_width_chars(GTK_ENTRY(documentEditor.operatorEntry), 10);
    gtk_table_attach_defaults(GTK_TABLE (table), documentEditor.operatorEntry, 1, 2, 0, 1);
    gtk_label_set_mnemonic_widget(GTK_LABEL (label), documentEditor.operatorEntry);
    gtk_entry_set_alignment(GTK_ENTRY (documentEditor.operatorEntry), 0.5);

    label = gtk_label_new("Date d'édition :");
    alignment = gtk_alignment_new(1, 0, 0, 0);
    gtk_container_add(GTK_CONTAINER(alignment), label);
    gtk_table_attach_defaults(GTK_TABLE (table), alignment, 0, 1, 1, 2);
    documentEditor.editDateEntry = gtk_entry_new();
    gtk_entry_set_editable(GTK_ENTRY (documentEditor.editDateEntry), FALSE);
    gtk_widget_set_can_focus(documentEditor.editDateEntry, FALSE);
    gtk_entry_set_width_chars(GTK_ENTRY(documentEditor.editDateEntry), 10);
    gtk_table_attach_defaults(GTK_TABLE (table), documentEditor.editDateEntry, 1, 2, 1, 2);
    gtk_label_set_mnemonic_widget(GTK_LABEL (label), documentEditor.editDateEntry);
    gtk_entry_set_max_length(GTK_ENTRY (documentEditor.editDateEntry), 10);
    gtk_entry_set_alignment(GTK_ENTRY (documentEditor.editDateEntry), 0.5);

    label = gtk_label_new("Date de fin de validité (pour un devis) :");
    alignment = gtk_alignment_new(1, 0, 0, 0);
    gtk_container_add(GTK_CONTAINER(alignment), label);
    gtk_table_attach_defaults(GTK_TABLE (table), alignment, 0, 1, 2, 3);
    documentEditor.expiryDateEntry = gtk_entry_new();
    gtk_table_attach_defaults(GTK_TABLE (table), documentEditor.expiryDateEntry, 1, 2, 2, 3);
    gtk_label_set_mnemonic_widget(GTK_LABEL (label), documentEditor.expiryDateEntry);
    gtk_entry_set_max_length(GTK_ENTRY (documentEditor.expiryDateEntry), 20);
    gtk_entry_set_alignment(GTK_ENTRY (documentEditor.expiryDateEntry), 0.5);
    if (typeAction == DISPLAY_DOCUMENT)
        gtk_widget_set_can_focus(documentEditor.expiryDateEntry, FALSE);

    label = gtk_label_new("Numéro de document :");
    alignment = gtk_alignment_new(1, 0, 0, 0);
    gtk_container_add(GTK_CONTAINER(alignment), label);
    gtk_table_attach_defaults(GTK_TABLE (table), alignment, 0, 1, 3, 4);
    documentEditor.docNumberEntry = gtk_entry_new();
    gtk_entry_set_editable(GTK_ENTRY (documentEditor.docNumberEntry), FALSE);
    gtk_widget_set_can_focus(documentEditor.docNumberEntry, FALSE);
    gtk_entry_set_width_chars(GTK_ENTRY(documentEditor.docNumberEntry), 10);
    gtk_table_attach_defaults(GTK_TABLE (table), documentEditor.docNumberEntry, 1, 2, 3, 4);
    gtk_label_set_mnemonic_widget(GTK_LABEL (label), documentEditor.docNumberEntry);
    gtk_entry_set_alignment(GTK_ENTRY (documentEditor.docNumberEntry), 0.5);

    /* objet
     customer + choix customer + data + n° */
    hbox = gtk_hbox_new(FALSE, 5);
    gtk_box_pack_start(GTK_BOX (GTK_DIALOG (dialog)->vbox), hbox, TRUE, TRUE, 5);

    label = gtk_label_new("Objet :");
    gtk_box_pack_start(GTK_BOX (hbox), label, FALSE, FALSE, 0);
    documentEditor.objectEntry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX (hbox), documentEditor.objectEntry, TRUE, TRUE, 0);
    gtk_label_set_mnemonic_widget(GTK_LABEL (label), documentEditor.objectEntry);
    gtk_entry_set_max_length(GTK_ENTRY (documentEditor.objectEntry), 200);
    if (typeAction == DISPLAY_DOCUMENT)
        gtk_widget_set_can_focus(documentEditor.objectEntry, FALSE);

    /* lignes de saisies + scrollbar */
    table = gtk_table_new(2 * (EDITOR_ROWCOUNT + 1), 11, FALSE);
    gtk_table_set_row_spacings(GTK_TABLE (table), 4);
    gtk_table_set_col_spacings(GTK_TABLE (table), 4);

    alignment = gtk_alignment_new(1, 0, 0, 0);
    gtk_container_add(GTK_CONTAINER(alignment), table);
    gtk_box_pack_start(GTK_BOX (GTK_DIALOG (dialog)->vbox), alignment, FALSE, FALSE, 5);

    gdk_color_parse("yellow", &color);

    label = gtk_label_new("Code");
    frame = gtk_event_box_new();
    gtk_widget_modify_bg(frame, GTK_STATE_NORMAL, &color);
    gtk_container_add(GTK_CONTAINER(frame), label);
    gtk_table_attach_defaults(GTK_TABLE (table), frame, 0, 1, 0, 1);
    label = gtk_label_new("Désignation");
    frame = gtk_event_box_new();
    gtk_widget_modify_bg(frame, GTK_STATE_NORMAL, &color);
    gtk_container_add(GTK_CONTAINER(frame), label);
    gtk_table_attach_defaults(GTK_TABLE (table), frame, 1, 8, 0, 1);
    label = gtk_label_new("Quantité");
    frame = gtk_event_box_new();
    gtk_widget_modify_bg(frame, GTK_STATE_NORMAL, &color);
    gtk_container_add(GTK_CONTAINER(frame), label);
    gtk_table_attach_defaults(GTK_TABLE (table), frame, 1, 2, 1, 2);
    label = gtk_label_new("Unité");
    frame = gtk_event_box_new();
    gtk_widget_modify_bg(frame, GTK_STATE_NORMAL, &color);
    gtk_container_add(GTK_CONTAINER(frame), label);
    gtk_table_attach_defaults(GTK_TABLE (table), frame, 2, 3, 1, 2);
    label = gtk_label_new("Prix de revient");
    frame = gtk_event_box_new();
    gtk_widget_modify_bg(frame, GTK_STATE_NORMAL, &color);
    gtk_container_add(GTK_CONTAINER(frame), label);
    gtk_table_attach_defaults(GTK_TABLE (table), frame, 3, 4, 1, 2);
    label = gtk_label_new("Prix de vente");
    frame = gtk_event_box_new();
    gtk_widget_modify_bg(frame, GTK_STATE_NORMAL, &color);
    gtk_container_add(GTK_CONTAINER(frame), label);
    gtk_table_attach_defaults(GTK_TABLE (table), frame, 4, 5, 1, 2);
    label = gtk_label_new("Réduction");
    frame = gtk_event_box_new();
    gtk_widget_modify_bg(frame, GTK_STATE_NORMAL, &color);
    gtk_container_add(GTK_CONTAINER(frame), label);
    gtk_table_attach_defaults(GTK_TABLE (table), frame, 5, 6, 1, 2);
    label = gtk_label_new("Prix public");
    frame = gtk_event_box_new();
    gtk_widget_modify_bg(frame, GTK_STATE_NORMAL, &color);
    gtk_container_add(GTK_CONTAINER(frame), label);
    gtk_table_attach_defaults(GTK_TABLE (table), frame, 6, 7, 1, 2);
    label = gtk_label_new("Taux TVA (en %)");
    frame = gtk_event_box_new();
    gtk_widget_modify_bg(frame, GTK_STATE_NORMAL, &color);
    gtk_container_add(GTK_CONTAINER(frame), label);
    gtk_table_attach_defaults(GTK_TABLE (table), frame, 7, 8, 1, 2);

    for (i = 0; i < EDITOR_ROWCOUNT; ++i) {
        {
            CatalogRecord_FieldProperties properties = CatalogRecord_getFieldProperties(
                    CATALOGRECORD_CODE_FIELD);
            documentEditor.codeEntry[i] = gtk_entry_new();
            gtk_entry_set_max_length(GTK_ENTRY (documentEditor.codeEntry[i]), properties.maxlength);
            gtk_table_attach_defaults(GTK_TABLE (table), documentEditor.codeEntry[i], 0, 1, 0 + 2
                    * (i + 1), 1 + 2 * (i + 1));
            g_signal_connect(G_OBJECT(documentEditor.codeEntry[i]), "focus-out-event", G_CALLBACK(DocumentEditor_handleFocusOut), &documentEditor);
            g_signal_connect(G_OBJECT(documentEditor.codeEntry[i]), "key-press-event", G_CALLBACK(DocumentEditor_handleFirstEntry), &documentEditor);
            gtk_entry_set_width_chars(GTK_ENTRY(documentEditor.codeEntry[i]), 10);
            if (typeAction == DISPLAY_DOCUMENT)
                gtk_widget_set_can_focus(documentEditor.codeEntry[i], FALSE);
        }

        {
            if (typeAction != DISPLAY_DOCUMENT) {
                documentEditor.catalogButton[i] = gtk_button_new_with_label("Rechercher");
                gtk_table_attach_defaults(GTK_TABLE (table), documentEditor.catalogButton[i], 0, 1,
                        1 + 2 * (i + 1), 2 + 2 * (i + 1));
                g_signal_connect(documentEditor.catalogButton[i], "clicked", G_CALLBACK(DocumentEditor_chooseProduct),&documentEditor);
            } else
                documentEditor.catalogButton[i] = NULL;
        }

        {
            CatalogRecord_FieldProperties properties = CatalogRecord_getFieldProperties(
                    CATALOGRECORD_DESIGNATION_FIELD);
            documentEditor.designationEntry[i] = gtk_entry_new();
            gtk_entry_set_max_length(GTK_ENTRY (documentEditor.designationEntry[i]),
                    properties.maxlength);
            gtk_table_attach_defaults(GTK_TABLE (table), documentEditor.designationEntry[i], 1, 8,
                    0 + 2 * (i + 1), 1 + 2 * (i + 1));
            g_signal_connect(G_OBJECT(documentEditor.designationEntry[i]), "focus-out-event", G_CALLBACK(DocumentEditor_handleFocusOut), &documentEditor);
            if (typeAction == DISPLAY_DOCUMENT)
                gtk_widget_set_can_focus(documentEditor.designationEntry[i], FALSE);
        }

        {
            documentEditor.quantityEntry[i] = gtk_entry_new();
            gtk_table_attach_defaults(GTK_TABLE (table), documentEditor.quantityEntry[i], 1, 2, 1
                    + 2 * (i + 1), 2 + 2 * (i + 1));
            gtk_entry_set_alignment(GTK_ENTRY (documentEditor.quantityEntry[i]), 1);
            g_signal_connect(G_OBJECT(documentEditor.quantityEntry[i]), "focus-out-event", G_CALLBACK(DocumentEditor_handleFocusOut), &documentEditor);
            gtk_entry_set_width_chars(GTK_ENTRY(documentEditor.quantityEntry[i]), 10);
            documentEditor.quantity_insert_text_handler[i]
                    = g_signal_connect(G_OBJECT(documentEditor.quantityEntry[i]), "insert_text", G_CALLBACK(DocumentEditor_insert_text_handler_positiveNumeric), &documentEditor);
            if (typeAction == DISPLAY_DOCUMENT)
                gtk_widget_set_can_focus(documentEditor.quantityEntry[i], FALSE);
        }

        {
            CatalogRecord_FieldProperties properties = CatalogRecord_getFieldProperties(
                    CATALOGRECORD_UNITY_FIELD);
            documentEditor.unityEntry[i] = gtk_entry_new();
            gtk_table_attach_defaults(GTK_TABLE (table), documentEditor.unityEntry[i], 2, 3, 1 + 2
                    * (i + 1), 2 + 2 * (i + 1));
            gtk_entry_set_max_length(GTK_ENTRY (documentEditor.unityEntry[i]), properties.maxlength);
            g_signal_connect(G_OBJECT(documentEditor.unityEntry[i]), "focus-out-event", G_CALLBACK(DocumentEditor_handleFocusOut), &documentEditor);
            gtk_entry_set_width_chars(GTK_ENTRY(documentEditor.unityEntry[i]), 10);
            if (typeAction == DISPLAY_DOCUMENT)
                gtk_widget_set_can_focus(documentEditor.unityEntry[i], FALSE);
        }

        {
            documentEditor.basePriceEntry[i] = gtk_entry_new();
            gtk_table_attach_defaults(GTK_TABLE (table), documentEditor.basePriceEntry[i], 3, 4, 1
                    + 2 * (i + 1), 2 + 2 * (i + 1));
            gtk_entry_set_alignment(GTK_ENTRY (documentEditor.basePriceEntry[i]), 1);
            g_signal_connect(G_OBJECT(documentEditor.basePriceEntry[i]), "focus-out-event", G_CALLBACK(DocumentEditor_handleFocusOut), &documentEditor);
            gtk_entry_set_width_chars(GTK_ENTRY(documentEditor.basePriceEntry[i]), 10);
            documentEditor.basePrice_insert_text_handler[i]
                    = g_signal_connect(G_OBJECT(documentEditor.basePriceEntry[i]), "insert_text", G_CALLBACK(DocumentEditor_insert_text_handler_positiveNumeric), &documentEditor);
            if (typeAction == DISPLAY_DOCUMENT)
                gtk_widget_set_can_focus(documentEditor.basePriceEntry[i], FALSE);
        }

        {
            documentEditor.sellingPriceEntry[i] = gtk_entry_new();
            gtk_table_attach_defaults(GTK_TABLE (table), documentEditor.sellingPriceEntry[i], 4, 5,
                    1 + 2 * (i + 1), 2 + 2 * (i + 1));
            gtk_entry_set_alignment(GTK_ENTRY (documentEditor.sellingPriceEntry[i]), 1);
            g_signal_connect(G_OBJECT(documentEditor.sellingPriceEntry[i]), "focus-out-event", G_CALLBACK(DocumentEditor_handleFocusOut), &documentEditor);
            gtk_entry_set_width_chars(GTK_ENTRY(documentEditor.sellingPriceEntry[i]), 10);
            documentEditor.sellingPrice_insert_text_handler[i]
                    = g_signal_connect(G_OBJECT(documentEditor.sellingPriceEntry[i]), "insert_text", G_CALLBACK(DocumentEditor_insert_text_handler_positiveNumeric), &documentEditor);
            if (typeAction == DISPLAY_DOCUMENT)
                gtk_widget_set_can_focus(documentEditor.sellingPriceEntry[i], FALSE);
        }

        {
            documentEditor.discountEntry[i] = gtk_entry_new();
            gtk_table_attach_defaults(GTK_TABLE (table), documentEditor.discountEntry[i], 5, 6, 1
                    + 2 * (i + 1), 2 + 2 * (i + 1));
            gtk_entry_set_alignment(GTK_ENTRY (documentEditor.discountEntry[i]), 1);
            g_signal_connect(G_OBJECT(documentEditor.discountEntry[i]), "focus-out-event", G_CALLBACK(DocumentEditor_handleFocusOut), &documentEditor);
            gtk_entry_set_width_chars(GTK_ENTRY(documentEditor.discountEntry[i]), 10);
            documentEditor.discount_insert_text_handler[i]
                    = g_signal_connect(G_OBJECT(documentEditor.discountEntry[i]), "insert_text", G_CALLBACK(DocumentEditor_insert_text_handler_positiveNumeric), &documentEditor);
            if (typeAction == DISPLAY_DOCUMENT)
                gtk_widget_set_can_focus(documentEditor.discountEntry[i], FALSE);
        }

        {
            documentEditor.finalPriceEntry[i] = gtk_entry_new();
            gtk_table_attach_defaults(GTK_TABLE (table), documentEditor.finalPriceEntry[i], 6, 7, 1
                    + 2 * (i + 1), 2 + 2 * (i + 1));
            gtk_entry_set_editable(GTK_ENTRY(documentEditor.finalPriceEntry[i]), FALSE);
            gtk_entry_set_alignment(GTK_ENTRY (documentEditor.finalPriceEntry[i]), 1);
            g_signal_connect(G_OBJECT(documentEditor.finalPriceEntry[i]), "focus-out-event", G_CALLBACK(DocumentEditor_handleFocusOut), &documentEditor);
            gtk_entry_set_width_chars(GTK_ENTRY(documentEditor.finalPriceEntry[i]), 10);
            gtk_widget_set_can_focus(documentEditor.finalPriceEntry[i], FALSE);
        }

        {
            documentEditor.rateOfVATEntry[i] = gtk_entry_new();
            gtk_table_attach_defaults(GTK_TABLE (table), documentEditor.rateOfVATEntry[i], 7, 8, 1
                    + 2 * (i + 1), 2 + 2 * (i + 1));
            gtk_entry_set_alignment(GTK_ENTRY (documentEditor.rateOfVATEntry[i]), 1);
            gtk_entry_set_width_chars(GTK_ENTRY(documentEditor.rateOfVATEntry[i]), 10);
            g_signal_connect(G_OBJECT(documentEditor.rateOfVATEntry[i]), "focus-out-event", G_CALLBACK(DocumentEditor_handleFocusOut), &documentEditor);
            g_signal_connect(G_OBJECT(documentEditor.rateOfVATEntry[i]), "key-press-event", G_CALLBACK(DocumentEditor_handleLastEntry), &documentEditor);
            documentEditor.rateOfVAT_insert_text_handler[i]
                    = g_signal_connect(G_OBJECT(documentEditor.rateOfVATEntry[i]), "insert_text", G_CALLBACK(DocumentEditor_insert_text_handler_positiveNumeric), &documentEditor);
            if (typeAction == DISPLAY_DOCUMENT)
                gtk_widget_set_can_focus(documentEditor.rateOfVATEntry[i], FALSE);
        }

        if (typeAction != DISPLAY_DOCUMENT) {
            documentEditor.insertBeforeButton[i] = gtk_button_new_with_label("Insérer avant");
            g_signal_connect(documentEditor.insertBeforeButton[i], "clicked", G_CALLBACK(DocumentEditor_insertBefore),&documentEditor);
            gtk_table_attach_defaults(GTK_TABLE (table), documentEditor.insertBeforeButton[i], 8,
                    9, 2 * (i + 1), 1 + 2 * (i + 1));

            documentEditor.insertAfterButton[i] = gtk_button_new_with_label("Insérer après");
            g_signal_connect(documentEditor.insertAfterButton[i], "clicked", G_CALLBACK(DocumentEditor_insertAfter),&documentEditor);
            gtk_table_attach_defaults(GTK_TABLE (table), documentEditor.insertAfterButton[i], 9,
                    10, 2 * (i + 1), 1 + 2 * (i + 1));

            documentEditor.removeButton[i] = gtk_button_new_with_label(
                    "Supprimer la ligne courante");
            g_signal_connect(documentEditor.removeButton[i], "clicked", G_CALLBACK(DocumentEditor_removeCurrent),&documentEditor);
            gtk_table_attach_defaults(GTK_TABLE (table), documentEditor.removeButton[i], 8, 10, 1
                    + 2 * (i + 1), 2 + 2 * (i + 1));
        } else {
            documentEditor.insertBeforeButton[i] = NULL;
            documentEditor.insertAfterButton[i] = NULL;
            documentEditor.removeButton[i] = NULL;
        }
    }

    adjustment = gtk_adjustment_new(0, 0, EDITOR_ROWCOUNT, 1, 1, EDITOR_ROWCOUNT);
    documentEditor.vscrollbar = gtk_vscrollbar_new(GTK_ADJUSTMENT(adjustment));
    g_signal_connect(G_OBJECT(documentEditor.vscrollbar), "value-changed",
            G_CALLBACK(DocumentEditor_scrollbarChanged), &documentEditor);
    g_signal_connect(G_OBJECT(documentEditor.vscrollbar), "change-value",
            G_CALLBACK(DocumentEditor_scrollbarBeforeChange), &documentEditor);
    gtk_table_attach_defaults(GTK_TABLE (table), documentEditor.vscrollbar, 10, 11, 2, 2
            * (EDITOR_ROWCOUNT + 1));

    /* totalHT, totalTVA, totalTTC */

    table = gtk_table_new(3, 2, FALSE);
    gtk_table_set_row_spacings(GTK_TABLE (table), 4);
    gtk_table_set_col_spacings(GTK_TABLE (table), 4);

    alignment = gtk_alignment_new(1, 0, 0, 0);
    gtk_container_add(GTK_CONTAINER(alignment), table);
    gtk_box_pack_start(GTK_BOX (GTK_DIALOG (dialog)->vbox), alignment, FALSE, FALSE, 0);

    {
        label = gtk_label_new("Total H.T. :");
        alignment = gtk_alignment_new(1, 0, 0, 0);
        gtk_container_add(GTK_CONTAINER(alignment), label);
        gtk_table_attach_defaults(GTK_TABLE (table), alignment, 0, 1, 0, 1);
        documentEditor.sumWithoutVATEntry = gtk_entry_new();
        gtk_table_attach_defaults(GTK_TABLE (table), documentEditor.sumWithoutVATEntry, 1, 2, 0, 1);
        gtk_label_set_mnemonic_widget(GTK_LABEL (label), documentEditor.sumWithoutVATEntry);
        gtk_entry_set_max_length(GTK_ENTRY (documentEditor.sumWithoutVATEntry), 200);
        gtk_entry_set_alignment(GTK_ENTRY (documentEditor.sumWithoutVATEntry), 1);
        gtk_entry_set_editable(GTK_ENTRY (documentEditor.sumWithoutVATEntry), FALSE);
        gtk_widget_set_can_focus(documentEditor.sumWithoutVATEntry, FALSE);
        gtk_entry_set_width_chars(GTK_ENTRY(documentEditor.sumWithoutVATEntry), 10);
    }

    {
        label = gtk_label_new("Total T.V.A. :");
        alignment = gtk_alignment_new(1, 0, 0, 0);
        gtk_container_add(GTK_CONTAINER(alignment), label);
        gtk_table_attach_defaults(GTK_TABLE (table), alignment, 0, 1, 1, 2);
        documentEditor.sumOfVATEntry = gtk_entry_new();
        gtk_table_attach_defaults(GTK_TABLE (table), documentEditor.sumOfVATEntry, 1, 2, 1, 2);
        gtk_label_set_mnemonic_widget(GTK_LABEL (label), documentEditor.sumOfVATEntry);
        gtk_entry_set_max_length(GTK_ENTRY (documentEditor.sumOfVATEntry), 200);
        gtk_entry_set_alignment(GTK_ENTRY (documentEditor.sumOfVATEntry), 1);
        gtk_entry_set_editable(GTK_ENTRY (documentEditor.sumOfVATEntry), FALSE);
        gtk_widget_set_can_focus(documentEditor.sumOfVATEntry, FALSE);
        gtk_entry_set_width_chars(GTK_ENTRY(documentEditor.sumOfVATEntry), 10);
    }

    {
        label = gtk_label_new("Total T.T.C. :");
        alignment = gtk_alignment_new(1, 0, 0, 0);
        gtk_container_add(GTK_CONTAINER(alignment), label);
        gtk_table_attach_defaults(GTK_TABLE (table), alignment, 0, 1, 2, 3);
        documentEditor.sumWithVATEntry = gtk_entry_new();
        gtk_table_attach_defaults(GTK_TABLE (table), documentEditor.sumWithVATEntry, 1, 2, 2, 3);
        gtk_label_set_mnemonic_widget(GTK_LABEL (label), documentEditor.sumWithVATEntry);
        gtk_entry_set_max_length(GTK_ENTRY (documentEditor.sumWithVATEntry), 200);
        gtk_entry_set_alignment(GTK_ENTRY (documentEditor.sumWithVATEntry), 1);
        gtk_entry_set_editable(GTK_ENTRY (documentEditor.sumWithVATEntry), FALSE);
        gtk_widget_set_can_focus(documentEditor.sumWithVATEntry, FALSE);
        gtk_entry_set_width_chars(GTK_ENTRY(documentEditor.sumWithVATEntry), 10);
    }

    gtk_widget_show_all(GTK_DIALOG (dialog)->vbox);
    DocumentEditor_loadData(&documentEditor, 0);

    do {
        response = gtk_dialog_run(GTK_DIALOG (dialog));
        first = (int) gtk_range_get_value(GTK_RANGE(documentEditor.vscrollbar));
        DocumentEditor_saveData(&documentEditor,first);
        if (response == 1) {
            Print_preview(document);
        }
    } while (response == 1);
    gtk_widget_destroy(dialog);

    return response == GTK_RESPONSE_OK;
}
