#ifndef COMMUNICATION_VIEW_H
#define COMMUNICATION_VIEW_H
#include <gtkmm.h>
#include <gtksourceviewmm.h>
#include <mutex>

class CommunicationView {
public:
    CommunicationView()
    : sig_NewMessageUART(){};

   ~CommunicationView(){};

    void init(Glib::RefPtr<Gtk::Builder> builder);

    // Send a string to UART/other
    void send_string(std::string);

    // Receives a string from UART/other (reads bufferUART)
    void receive_string(std::string);

    // Clears the text view
    void clear_view();


private:
    Gsv::View*                  m_sourceView;
    Glib::RefPtr<Gsv::Buffer>   m_sourceBuffer;
    Glib::RefPtr<Gsv::Buffer::TagTable> m_tagTable;

    // String reception
    void print_UART_buffer();
    std::mutex  bufferUART_mutex;
    std::string bufferUART;
    Glib::Dispatcher sig_NewMessageUART;

    // String send
    Gtk::Entry* m_Entry;

    class ModelColumns : public Gtk::TreeModel::ColumnRecord {
    public:
        ModelColumns() { add(m_col_id); add(m_col_name); }
        Gtk::TreeModelColumn<unsigned int>  m_col_id;
        Gtk::TreeModelColumn<Glib::ustring> m_col_name;
    };
    ModelColumns m_Columns;

    void on_insert();

    // Serial port connection
    void refreshSerialPorts();
    void onConnectPort();
    Gtk::ComboBoxText* serialPortsList;

};

#endif
