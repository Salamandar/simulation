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

    void clear();

    // Adds to the buffer
    void append_received_line(std::string);

private:
    Gsv::View*                  m_sourceView;
    Glib::RefPtr<Gsv::Buffer>   m_sourceBuffer;
    Glib::RefPtr<Gsv::Buffer::TagTable> m_tagTable;


    void print_UART_buffer();
    std::mutex  bufferUART_mutex;
    std::string bufferUART;
    Glib::Dispatcher sig_NewMessageUART;


    Gtk::Entry*                     m_Entry;
    Glib::RefPtr<Gtk::EntryBuffer>  m_entryBuffer;

    class ModelColumns : public Gtk::TreeModel::ColumnRecord {
    public:
        ModelColumns() { add(m_col_id); add(m_col_name); }
        Gtk::TreeModelColumn<unsigned int> m_col_id;
        Gtk::TreeModelColumn<Glib::ustring> m_col_name;
    };
    ModelColumns m_Columns;
    typedef std::map<int, Glib::ustring> type_actions_map;
    type_actions_map m_CompletionActions;

    void on_insert();
};

#endif
