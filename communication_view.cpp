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
    m_sourceBuffer  = m_sourceView->get_source_buffer();
    m_tagTable      = m_sourceBuffer->get_tag_table();
    m_sourceBuffer->signal_insert().connect(
        sigc::mem_fun(*this, &CommunicationView::on_insert));

    builder->get_widget("CommunicationEntry", m_Entry);
    m_entryBuffer  = m_Entry->get_buffer();
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

void CommunicationView::on_insert(const Gtk::TextBuffer::iterator& pos, const Glib::ustring& text, int bytes) {
    std::cout<< text.data()[0] <<std::flush;
    append_to_UART(text.data()[0]);
}
