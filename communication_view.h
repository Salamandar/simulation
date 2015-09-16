#ifndef COMMUNICATION_VIEW_H
#define COMMUNICATION_VIEW_H
#include <gtkmm.h>
#include <gtksourceviewmm.h>

class CommunicationView {
public:
    CommunicationView(){};

   ~CommunicationView(){};

    void init(Glib::RefPtr<Gtk::Builder> builder);

    void clear();

private:
    Gsv::View*                  m_sourceView;
    Glib::RefPtr<Gsv::Buffer>   m_sourceBuffer;


    Gtk::ScrolledWindow *m_sourceScrolledWindow;

    Glib::RefPtr<Gtk::TextBuffer::Tag> m_sourceTags[3];
    Glib::RefPtr<Gtk::TextBuffer::TagTable> m_tagTable;

//    Gdk::Color m_historyColors[3];
//    Gdk::Color m_backgroundColor;
};

#endif
