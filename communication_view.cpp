#include "communication_view.h"

void CommunicationView::init(Glib::RefPtr<Gtk::Builder> builder) {
    builder->get_widget("CommunicationSourceView", m_sourceView);

    m_tagTable = Gtk::TextBuffer::TagTable::create();

    //m_emptyBuffer = Gsv::Buffer::create(m_tagTable);

}
