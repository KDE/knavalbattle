
// include files for Qt
#include <qprinter.h>
#include <qpainter.h>

// application specific includes
#include "kbattleshipview.h"
#include "kbattleshipdoc.h"
#include "kbattleship.h"

#include "kbattleshipview.moc"

KBattleshipView::KBattleshipView(QWidget *parent, const char *name) : QWidget(parent, name)
{
  setBackgroundMode(PaletteBase);
}

KBattleshipView::~KBattleshipView()
{
}

KBattleshipDoc *KBattleshipView::getDocument() const
{
  KBattleshipApp *theApp=(KBattleshipApp *) parentWidget();

  return theApp->getDocument();
}

void KBattleshipView::print(QPrinter *pPrinter)
{
  QPainter printpainter;
  printpainter.begin(pPrinter);
	
  // TODO: add your printing code here

  printpainter.end();
}
