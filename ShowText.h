#ifndef SHOW_H
#define SHOW_H

#include <QObject>
#include <QString>
#include <QtMath>
class ShowText: public QObject
{
  Q_OBJECT
  Q_PROPERTY(QString text READ text WRITE setText NOTIFY TextChanged)

public:
  ShowText(QObject *parent = 0);

  void  setText(QString test);

  QString  text();

  QString  setMultiplyRanking(QString string);

  QString  setSumRanking(QString string);

  int  getNextOperation(QString string, int index);

  int  getPreviousOperation(QString string, int index);

  int  getPreviousOperationWithoutNegative(QString string, int index);

  QString  setPowerRanking(QString string, int indexPower);

  bool  IsOperation(QString string, int indexOP);

  bool  IsOperationWithoutPower(QString string, int indexOP);

  int  getNextOperationWithoutNegative(QString string, int indexOperation);

  bool  isCorrect(QString string);

signals:
  void  TextChanged();

  void  KeyBoardChanged();

public slots:
  void  SlotsText();

  void  appendHistory(QString, bool isBackSpace = false);/// append number in history

  bool  undoHistoryCheck();

  bool  redoHistoryCheck();// 1 and  0 range

  QString  undoHistorySlots();

  QString  redoHistorySlots();

  QString  mResult(QString string);

  QString  getDisplayText(QString index, QString text);

  QString  getIndex();

  void  setIndex(QString index);

protected:
private:
  QString      _text;// my text in board
  QString      _index;
  QStringList  _textListHistory;// for Undo
  QStringList  _textListDeletedHistory;// for redo
};

#endif // SHOW_H
