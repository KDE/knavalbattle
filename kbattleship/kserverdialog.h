#ifndef KSERVERDIALOG_H
#define KSERVERDIALOG_H

#include <kdebug.h>
#include "dialogs/serverDlg.h"

class KServerDialog : startServerDlg
{
    Q_OBJECT
    public:
        KServerDialog( QWidget *parent = 0, const char *name = 0 );
        ~KServerDialog();
	
    public slots:
        void slotStartClicked();
};

#endif
