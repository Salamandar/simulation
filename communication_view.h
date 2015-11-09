#ifndef COMMUNICATION_VIEW_H
#define COMMUNICATION_VIEW_H
#include <gtkmm.h>
#include <gtksourceviewmm.h>
#include <string.h>

class CommunicationView {
public:
    CommunicationView(){};

   ~CommunicationView(){};

    void init(Glib::RefPtr<Gtk::Builder> builder);

    void clear();

    void append_received_line(const std::string ligne);

private:
    Gsv::View*                  m_sourceView;
    Glib::RefPtr<Gsv::Buffer>   m_sourceBuffer;
    Glib::RefPtr<Gsv::Buffer::TagTable> m_tagTable;

    Gtk::Entry*                 m_Entry;
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
    void on_completion_action_activated(int index);

    Gtk::ScrolledWindow *m_sourceScrolledWindow;

    void on_insert();
};

#endif
