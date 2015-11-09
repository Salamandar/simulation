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

    Gtk::ScrolledWindow *m_sourceScrolledWindow;

    void on_insert();
};

#endif
