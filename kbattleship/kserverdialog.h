#ifndef KSERVERDIALOG_H
#define KSERVERDIALOG_H

#include "dialogs/serverDlg.h"

class KServerDialog : public QWidget
{
    Q_OBJECT
    public:
        KServerDialog( QWidget *parent = 0, const char *name = 0 );
        ~KServerDialog();
    public slots:
        void slotStartClicked();
};

#endif
