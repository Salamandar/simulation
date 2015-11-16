#include <iostream>
#include "communication_view.h"

// Autocomplétion
#include "../common_code/communication/s2a.h"
extern char *s2a_keys[S2A_SIZE];

void CommunicationView::init(Glib::RefPtr<Gtk::Builder> builder) {
    Gtk::Button* button;
    builder->get_widget("SerialRefreshButton", button);
    button->signal_clicked().connect(sigc::mem_fun(*this,
        &CommunicationView::refreshSerialPorts));
    builder->get_widget("SerialConnectButton", button);
    button->signal_clicked().connect(sigc::mem_fun(*this,
        &CommunicationView::onConnectPort));
    builder->get_widget("SerialPortsList", serialPortsList);


    builder->get_widget("CommunicationSourceView", m_sourceView);
    builder->get_widget("CommunicationEntry", m_Entry);
    m_sourceBuffer  = m_sourceView->get_source_buffer();
    m_tagTable      = m_sourceBuffer->get_tag_table();
    m_Entry->signal_activate().connect(
        sigc::mem_fun(*this, &CommunicationView::on_insert));

    sig_NewMessageUART.connect(sigc::mem_fun(*this,
        &CommunicationView::print_UART_buffer));

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
}


void CommunicationView::clear_view() {
    m_sourceBuffer->set_text("");
}

void CommunicationView::print_UART_buffer() {
    bufferUART_mutex.lock();
    //if (bufferUART.back() != '\n')
    //    bufferUART.append("\n");

    m_sourceBuffer->insert(
        m_sourceBuffer->get_iter_at_offset(-1),
        bufferUART);
    bufferUART.clear();
    bufferUART_mutex.unlock();
    m_sourceBuffer->place_cursor(m_sourceBuffer->get_iter_at_offset(-1));
    m_sourceView->scroll_to(m_sourceBuffer->get_insert());
}


void CommunicationView::receive_string(std::string ligne) {
    bufferUART_mutex.lock();

    bufferUART+=ligne;
    sig_NewMessageUART.emit();

    bufferUART_mutex.unlock();
}

void CommunicationView::on_insert() {
    communicator.send(m_Entry->get_text().raw());
    receive_string   (m_Entry->get_text().raw()+"\n");
}

void CommunicationView::refreshSerialPorts() {
    vector<string> l =SerialComm::list_open_ports();
    vector<string>::iterator it = l.begin();
    serialPortsList->remove_all();
    while (it != l.end()) {
        serialPortsList->append(*it);
        it++;
    }
    serialPortsList->set_active(0);
}
void CommunicationView::onConnectPort() {
    communicator.connect(serialPortsList->get_active_text());
}
