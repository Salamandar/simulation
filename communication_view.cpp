#include "communication_view.h"

void CommunicationView::init(Glib::RefPtr<Gtk::Builder> builder) {
    builder->get_widget("CommunicationSourceView", m_sourceView);
    m_sourceBuffer  = m_sourceView->get_source_buffer();
    m_tagTable      = m_sourceBuffer->get_tag_table();
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
}
