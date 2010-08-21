$EXTRACTRC *.rc *.kcfg >> rc.cpp
$XGETTEXT `find . -name "*.cpp"` -o $podir/kbattleship.pot

