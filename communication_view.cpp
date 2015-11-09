#include "communication_view.h"
#include <iostream>

#include "../common_code/communication/s2a.h"
extern char *s2a_keys[S2A_SIZE];

#ifdef __cplusplus
extern "C" {
#endif

void append_to_UART(unsigned char c);

#ifdef __cplusplus
}
#endif


void CommunicationView::init(Glib::RefPtr<Gtk::Builder> builder) {
    builder->get_widget("CommunicationSourceView", m_sourceView);
    builder->get_widget("CommunicationEntry", m_Entry);
    m_sourceBuffer  = m_sourceView->get_source_buffer();
    m_tagTable      = m_sourceBuffer->get_tag_table();
    m_Entry->signal_activate().connect(
        sigc::mem_fun(*this, &CommunicationView::on_insert));

    // Tout ça pour la complétion
    Glib::RefPtr<Gtk::EntryCompletion>
        completion = Gtk::EntryCompletion::create();
    Glib::RefPtr<Gtk::ListStore>
        refCompletionModel = Gtk::ListStore::create(m_Columns);

    m_Entry->set_completion(completion);
    completion->set_inline_completion(true);
    completion->set_inline_selection(true);
    completion->set_minimum_key_length(0);
    completion->set_model(refCompletionModel);

    Gtk::TreeModel::Row row = *(refCompletionModel->append());
    for (int i = 0; i <= S2A_FCT_MAX_INDEX; ++i) {
        row[m_Columns.m_col_id]   = i;
        row[m_Columns.m_col_name] = s2a_keys[i];
        row = *(refCompletionModel->append());
    }

    completion->set_text_column(m_Columns.m_col_name);
    for(type_actions_map::iterator iter = m_CompletionActions.begin();
        iter != m_CompletionActions.end();
        ++iter) {
        int position = iter->first;
        Glib::ustring title = iter->second;
        completion->insert_action_text(title, position);
    }
}


void CommunicationView::clear() {
    m_sourceBuffer->set_text("");
}

void CommunicationView::append_received_line(std::string ligne) {
    if (ligne.back() != '\n')
        ligne.append("\n");


    m_sourceBuffer->insert(
        m_sourceBuffer->get_iter_at_offset(-1),
        ligne);
    m_sourceBuffer->place_cursor(m_sourceBuffer->get_iter_at_offset(-1));
    m_sourceView->scroll_to(m_sourceBuffer->get_insert());
}

void CommunicationView::on_insert() {
    std::string texte = m_Entry->get_text().raw();
    for(char& c : texte)
        append_to_UART(c);
    append_to_UART('\n');

    append_received_line(texte);
}
