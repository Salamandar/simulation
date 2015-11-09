#include "communication_view.h"
#include <iostream>

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
    //m_sourceView->scroll_to(m_sourceBuffer->get_insert());
}

void CommunicationView::on_insert() {
    std::string texte = m_Entry->get_text().raw();
    for(char& c : texte)
        append_to_UART(c);
    append_to_UART('\n');

    append_received_line(texte);
}
