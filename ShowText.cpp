#include "ShowText.h"
#include <QtDebug>

ShowText::ShowText(QObject *parent):
  QObject(parent)
{
  connect(this, &ShowText::TextChanged, this, &ShowText::SlotsText);
// auto  shortcut = new QShortcut(QKeySequence(tr("Ctrl+O", "File|Open")),
// parent);
}

void  ShowText::setText(QString test)
{
  _text = test;
}

QString  ShowText::text()
{
  return _text;
}

void  ShowText::SlotsText()
{
  qInfo() << _text << "clicked func";
}

void  ShowText::appendHistory(QString string, bool isBackSpace)
{
  bool  itemExist = false;

// for (QString item : _textListHistory)
// {
// if (item == string)
// {
// itemExist = true;
// }
// }

  for (int i = 1; i < _textListHistory.size(); ++i)
  {
    if (_textListHistory.at(i) == _textListHistory.at(i - 1))
    {
      itemExist = true;
    }
  }

  if (itemExist == false)
  {
    _textListHistory.append(string);
    qInfo() << string << "item Exist";
    _textListDeletedHistory.clear();// delete last redo exist
  }
}

bool  ShowText::undoHistoryCheck()
{
  if (_textListHistory.size() == 0)
  {
    return false;
  }

  return true;
}

bool  ShowText::redoHistoryCheck()
{
  if (_textListDeletedHistory.size() == 0)
  {
    return false;
  }

  return true;
}

QString  ShowText::undoHistorySlots()
{
  _textListDeletedHistory.append(_textListHistory.last());

  _textListHistory.removeLast();

  if (_textListHistory.size() == 0)
  {
    return QString("");
  }
  else
  {
    return _textListHistory.back();
  }

// _textListHistory.removeLast();

  // return mLast;
}

QString  ShowText::redoHistorySlots()
{
  _textListHistory.append(_textListDeletedHistory.last());

// _textListHistory.removeLast();
  QString  mList = _textListDeletedHistory.last();

  if (_textListDeletedHistory.size() == 0)
  {
    return QString("");
  }
  else
  {
    _textListDeletedHistory.removeLast();

    return mList;
  }
}

QString  ShowText::mResult(QString string)
{
  if (isCorrect(string) == false)
  {
    return "Eror";
  }

  QString  result = ""; // my end result (return it)

  if (string.size() == 0)// is Empty()?
  {
    qInfo() << "its Empty";
    qInfo() << "string is empty";

    return "its Empty";
  }

  if ((string.at(string.size() - 1) == "√") || (string.at(string.size() - 1) == "-") || (string.at(string.size() - 1) == "+")
      || (string.at(string.size() - 1) == "x") || (string.at(string.size() - 1) == "/"))
  {
    return "Eror";
  }

  if ((string.at(0) == '+') || (string.at(0) == '-') || (string.at(0) == 'x') || (string.at(0) == '/') || (string.at(0) == '%'))
  {
    return "Eror";
  }

  if (string.size() == 0)// is Empty()?
  {
    qInfo() << "its Empty";
    qInfo() << "string is empty";

    return "its Empty";
  }

  if ((string.at(0) != "(") || (string.at(string.size() - 1) != ")"))
  {
    qInfo() << "convert" << string << "to" << '(' + string + ')';
    string = '(' + string + ')';
  }

  QList<int>  indexOParentheses;/// )
  QList<int>  indexCParentheses;/// (
  QList<int>  indexPositive;/// +
  QList<int>  indexNegative;/// -
  QList<int>  indexMultyPly;/// x
  QList<int>  indexDevide;/// -/-
  QList<int>  indexPower;/// x²
  QList<int>  indexPercentage;/// x²
  QList<int>  indexRadical;/// x²
  QList<int>  indexRadicalWithParanthese;/// x²

  for (int i = 0; i < string.size(); ++i)
  {
    if (string.at(i) == '(')/// open parenthreses list of index (index O Paranthreases)
    {
      indexOParentheses.append(i);
    }

    if (string.at(i) == ')')// clsoe parenthreses list of index (index C Paranthreases)
    {
      indexCParentheses.append(i);

      if (string.at(i - 1) == '(')/// example ("empty") (Eror)
      {
        qInfo() << "Eror(wrong parantheses) Code: 3";

        return "EROR";
      }
    }

    if (string.at(i) == '+')// positive list of index
    {
      indexPositive.append(i);
    }

    if (string.at(i) == '-')// negative list of index
    {
      if (getPreviousOperation(string, i) == i)
      {
        qInfo() << "dont append to indexNegative !!(-5)";
        indexNegative.append(i);
      }
    }

    if (string.at(i) == 'x')// multiply list of index
    {
      indexMultyPly.append(i);
    }

    if (string.at(i) == '/')// divide list of index
    {
      indexDevide.append(i);
    }

    if (string.at(i) == "²") // x²
    {
      indexPower.append(i);
    }

    if (string.at(i) == "%")// % list of index
    {
      indexPercentage.append(i);
    }

    if (string.at(i) == "√")
    {
      if (string.at(i + 1) == "²")
      {
        return "Eror";
      }

      if (string.at(i + 1) == "(")
      {
        indexRadicalWithParanthese.append(i);
      }
      else
      {
        indexRadical.append(i);
      }
    }

    if (indexCParentheses.size() > indexOParentheses.size()) /// example : ))( or ((5)))  (Eror)
    {
      qInfo() << "Eror(wrong parantheses) Code: 2";

      return "EROR";
    }
  }

  for (int i = 0; i < indexPercentage.size(); ++i)
  {
    string.replace(indexPercentage.at(i), 1, "/(100)");
    qInfo() << "replace %100 to" << string;

    return mResult(string);
  }

  for (int i = 1; i < indexOParentheses.size(); ++i) // i != 0 because we always have 1 more parenthese
  {
    bool  ok = IsOperation(string, indexOParentheses.at(i) - 1); // ok is true when before OP has number 5(y) -> (5)x(y)

    if (ok == true)
    {
      int      startOfNumber = getPreviousOperation(string, indexOParentheses.at(i));
      QString  filterStr;
      QString  newStr;

      for (int h = startOfNumber; h < indexOParentheses.at(i); ++h)
      {
        filterStr.append(string.at(h));
      }

      for (int h = 0; h < string.size(); ++h)
      {
        if (h < startOfNumber)
        {
          newStr.append(string.at(h));
        }
        else if (h == startOfNumber)
        {
          newStr.append("(" + filterStr + ")x");
        }
        else if (h >= indexOParentheses.at(i))
        {
          newStr.append(string.at(h));
        }
      }

      qInfo() << string << "was converted to correct Operation" << newStr;

      return mResult(newStr);
    }
  }

  for (int i = 0; i < indexCParentheses.size() - 1; ++i) // i != 0 because we always have 1 more parenthese
  {
    bool  ok = IsOperationWithoutPower(string, indexCParentheses.at(i) + 1); // ok is true when before OP has number

    if (ok == true)
    {
      int      endOfNumber = getNextOperation(string, indexCParentheses.at(i));
      QString  filterStr;
      QString  newStr;

      for (int h = indexCParentheses.at(i) + 1; h <= endOfNumber; ++h)
      {
        filterStr.append(string.at(h));
      }

      for (int h = 0; h < string.size(); ++h)
      {
        if (h < indexCParentheses.at(i) + 1)
        {
          newStr.append(string.at(h));
        }
        else if (h == indexCParentheses.at(i) + 1)
        {
          newStr.append("x(" + filterStr + ")");
        }
        else if (h >= endOfNumber)
        {
          newStr.append(string.at(h));
        }
      }

      qInfo() << string << "was converted to correct Operation" << newStr;

      return mResult(newStr);
    }
  }

  for (int i = 0; i < indexRadical.size(); ++i)
  {
    int      endIndexOfRadical = getNextOperationWithoutNegative(string, indexRadical.at(i));
    QString  newStr;
    qInfo() << "string" << string << "rad" << indexRadical.at(i) << "parenthese" << endIndexOfRadical;

    for (int h = indexRadical.at(i) + 1; h <= endIndexOfRadical; ++h)
    {
      newStr.append(string.at(h)); // new str all number in radical
    }

    double  result = qSqrt(newStr.toDouble());
    qInfo() << "under radical: " << newStr << "result: " << result;
    string.replace(indexRadical.at(i), endIndexOfRadical - indexRadical.at(i) + 1, QString::number(result));
    qInfo() << "full string" << string;

    return mResult(string);
  }

  for (int i = 0; i < indexPower.size(); ++i)   // power (list of index)
  {
    int      indexOfCompareOC = 0;
    QString  firstNumber;
    qInfo() << "power index " << indexPower.at(i);   // always 1 more for first open parenthese

    if (indexPower.at(i) != 1)
    {
      qInfo() << "myString" << string;

      if (string.at(indexPower.at(i) - 1) == ")")// way number 1 for power
      {
        if (string.at(indexPower.at(i) + 1) == "\u00b2")
        {
          qInfo() << "way number 1 for power";
          int  powerCount = 0;

          for (int z = indexPower.at(i); z < string.size(); ++z)
          {
            if (string.at(z) == "\u00b2")
            {
              ++powerCount;
            }
            else
            {
              z = string.size();
            }
          }

          int     newPowerCount        = powerCount;
          double  powerAfterParenthese = 2;

          while (powerCount != 1)
          {
            powerAfterParenthese = qPow(powerAfterParenthese, 2);
            --powerCount;
          }

          qInfo() << "my String" << string;

          int  counterOPower = 0;   // count open parenthese for power
          int  counterCPower = 0;  // count close parenthese for power

          for (int c = indexPower.at(i) - 1; c >= 0; --c)
          {
            if (string.at(c) == ")")
            {
              ++counterCPower;
            }

            if (string.at(c) == "(")
            {
              ++counterOPower;
            }

            if (counterCPower == counterOPower)
            {
              indexOfCompareOC = c;   // first open parantheses
              // qInfo() << "comp" << indexOfCompareOC;
              c = indexPower.size();
              QString  newStr;

              for (int h = indexOfCompareOC; h < indexPower.at(i); ++h)
              {
                firstNumber.append(string.at(h));   // first number
              }

              qInfo() << "string: " << string;

              for (int v = 0; v < string.size(); ++v)   // get all string in parenthese
              {
                if (v < indexOfCompareOC)
                {
                  newStr.append(string.at(v));
                }
                else if (v >= indexPower.at(i) + newPowerCount)
                {
                  newStr.append(string.at(v));
                }
                else if (v == indexOfCompareOC)
                {
                  if ((firstNumber.at(0) != "(") || (firstNumber.at(firstNumber.size() - 1) != ")"))
                  {
                    firstNumber = '(' + firstNumber + ')';
                  }

                  while (powerAfterParenthese != 0)
                  {
                    if (powerAfterParenthese == 1)
                    {
                      newStr.append(firstNumber);
                      --powerAfterParenthese;
                    }
                    else
                    {
                      newStr.append(firstNumber + "x");
                      --powerAfterParenthese;
                    }
                  }
                }
              }

              qInfo() << "result in power Operation" << newStr;

              return mResult(newStr);  // we changed power to normal operation!! now back and start again
            }
          }
        }
      }

      if (string.at(indexPower.at(i) - 1) != ")")// way number 2 for power
      {
        qInfo() << "way number 2 for power";
        string = setPowerRanking(string, indexPower.at(i));

        return mResult(string);
      }

      if (string.at(indexPower.at(i) - 1) == ")")// way number 3 for power
      {
        qInfo() << "way number 3 for power";
        int  counterOPower = 0;   // count open parenthese for power
        int  counterCPower = 0;  // count close parenthese for power

        for (int c = indexPower.at(i) - 1; c >= 0; --c)
        {
          if (string.at(c) == ")")
          {
            ++counterCPower;
          }

          if (string.at(c) == "(")
          {
            ++counterOPower;
          }

          if (counterCPower == counterOPower)
          {
            indexOfCompareOC = c;   // first open parantheses
// qInfo() << "comp" << indexOfCompareOC;
            c = indexPower.size();
            QString  newStr;

            for (int h = indexOfCompareOC; h < indexPower.at(i); ++h)
            {
              firstNumber.append(string.at(h));   // first number
            }

            for (int v = 0; v < string.size(); ++v)   // get all string in parenthese
            {
              if (v < indexOfCompareOC)
              {
                newStr.append(string.at(v));
              }
              else if (v > indexPower.at(i))
              {
                newStr.append(string.at(v));
              }
              else if (v == indexOfCompareOC)
              {
                if ((firstNumber.at(0) != "(") || (firstNumber.at(firstNumber.size() - 1) != ")"))
                {
                  firstNumber = '(' + firstNumber + ')';
                }

                newStr.append("((" + firstNumber + ')' + "x" + '(' + firstNumber + "))");
              }
            }

            return mResult(newStr);  // we changed power to normal operation!! now back and start again
          }
        }
      }

// if (string.at(indexPower.at(i) - 1) != ")")// way number 4 for power
// {
// qInfo() << "way number 4 for power";
// int  indexOfPreviousOperation = getPreviousOperationWithoutNegative(string, indexPower.at(i));   /// first number(index) between operation

// for (int h = indexOfPreviousOperation; h < indexPower.at(i); ++h)
// {
// firstNumber.append(string.at(h));   // first number
// }

// if ((firstNumber.at(0) != "(") || (firstNumber.at(firstNumber.size() - 1) != ")"))
// {
// firstNumber = firstNumber;
// }

// firstNumber = '(' + '(' + firstNumber + ')' + "x" + '(' + firstNumber + ')' + ')';
// qInfo() << firstNumber << "firstnumber in power";
// QString  stringFilter;

// for (int h = 0; h < string.size(); ++h)
// {
// if (h < indexOfPreviousOperation)
// {
// stringFilter.append(string.at(h));
// }
// else if (h > indexPower.at(i))
// {
// stringFilter.append(string.at(h));
// }
// else if (h == indexPower.at(i))
// {
// stringFilter.append(firstNumber);
// }
// }

// string = stringFilter;
// qInfo() << "result of Power" << string;

// return mResult(string);
// }
    }
  }

  if (indexCParentheses.size() != indexOParentheses.size())  /// openparenthese.size == closeParenthese.size
  {
    qInfo() << "Eror(wrong parantheses) Code: 1";

    return "EROR";
  }

  std::sort(indexOParentheses.begin(), indexOParentheses.end());
  std::sort(indexCParentheses.begin(), indexCParentheses.end());

  for (int i = 0; i < indexOParentheses.size(); ++i) // index of QList OpenParanthese
  {
    int  counterO = 0; // number of Open
    int  counterC = 0; // number of close
    int  index    = indexOParentheses.at(i);

    for (index; index < string.size(); ++index) // number of Open & Close Paranthese //wait when you got one Paranthese!!
    {
      if (string.at(index) == '(')
      {
        ++counterO;
      }

      if (string.at(index) == ')')
      {
        ++counterC;
      }

      if (counterC == counterO)
      {
        index = string.size();// cancel index FOR!!
      }
    }

// qInfo() << counterC << "   " << counterO;

    if (counterC == 1) // start one of the paranthese
    {
      int  countToEndAllParenthese = 0;

      for (int z = 0; z < indexCParentheses.size(); ++z)// index of QListCP
      {
        qInfo() << "String" << string;
        int  indexOfO = indexOParentheses.at(i); // currnet open parentheses
        int  indexOfC = indexCParentheses.at(z);  // currnet close parentheses
        int  itemO    = indexOParentheses.at(i); // currnet open parentheses (changed in loop)
        int  itemC    = indexCParentheses.at(z); // currnet  clsoe parentheses (changed in loop
// qInfo() << itemC << "  " << itemO;

        if (itemC > itemO)// close index more than open index
        {
          QString  mStr; // have all operation between paranthese

          for (itemO; itemO <= itemC; ++itemO) // add all between 2 paranthese (mStr)
          {
            mStr.append(string.at(itemO));
          }

          z = indexCParentheses.size();

// qInfo() << mStr << "!!";
          QString  rankingFilter = setMultiplyRanking(mStr); // set multiply in paranthese
          rankingFilter = setSumRanking(rankingFilter); // set sum in paranthese
          QString  result = "";

          for (int o = 0; o < indexOfO; ++o) // write result (result in paranthese)
          {
            result.append(string.at(o));
          }

          result.append(rankingFilter);

          for (int o = indexOfC + 1; o < string.size(); ++o)
          {
            result.append(string.at(o));
          }

          qInfo() << result;

          if (indexOParentheses.size() != counterC)
          {
            return mResult(result); // do for next paranthese
          }
          else
          {
            return result; // full result (end)
          }
        }
      }
    }
  }

  return mResult(result);
}

QString  ShowText::getDisplayText(QString index, QString clickText)
{
  QString  newText;

  for (int i = 0; i < _text.size(); ++i)
  {
    if (i < index.toInt())
    {
      newText.append(_text.at(i));
    }
    else if (i == index.toInt())
    {
      newText.append(clickText);
      newText.append(_text.at(i));
    }
    else if (i > index.toInt())
    {
      newText.append(_text.at(i));
    }
  }

  qInfo() << "my Display index" << index;

  _text = newText;

  return _text;
}

QString  ShowText::getIndex()
{
}

void  ShowText::setIndex(QString index)
{
}

QString  ShowText::setPowerRanking(QString string, int indexPower)
{
  qInfo() << "first Power : " << indexPower;// start of Power
  int      count       = 0;
  double   powerResult = 2;
  QString  myNumber;
  QString  newStr;
  int      startIndexOfNumber = getPreviousOperationWithoutNegative(string, indexPower - 1);
  int      lastIndexOfNumber  = getNextOperation(string, indexPower - 1);

  for (int z = indexPower; z < string.size(); ++z)
  {
    if (string.at(z) == "\u00b2")
    {
      ++count;
    }
    else
    {
      while (count != 1)
      {
        powerResult = qPow(powerResult, 2);
        --count;
      }

      qInfo() << "power Result : " << powerResult;

      for (int i = startIndexOfNumber; i < indexPower; ++i)
      {
        myNumber.append(string.at(i));
      }

      myNumber = QString::number(qPow(myNumber.toDouble(), powerResult));
      qInfo() << "my number from power" << myNumber;

      qInfo() << startIndexOfNumber << lastIndexOfNumber;

      z = string.size();
    }
  }

  for (int i = 0; i < string.size(); ++i)
  {
    if (i < startIndexOfNumber)
    {
      newStr.append(string.at(i));
    }

    if (i == startIndexOfNumber)
    {
      newStr.append(myNumber);
    }

    if (i > lastIndexOfNumber)
    {
      newStr.append(string.at(i));
    }
  }

  qInfo() << "ranking power result" << newStr;

  return newStr;
}

QString  ShowText::setMultiplyRanking(QString string)
{
  if (string.at(0) == '(') // remove open paranthese from current string (the innermost paranthese)
  {
    string.remove(0, 1);
  }

  if (string.at(string.size() - 1) == ')') // remove close paranthese from current string (the innermost paranthese)
  {
    string.remove(string.size() - 1, string.size());
  }

// qInfo() << string;

  QList<int>  counterMOrP;/// MultiPly or devide

  for (int i = 0; i < string.size(); ++i)
  {
    if ((string.at(i) == 'x') || (string.at(i) == '/'))
    {
      counterMOrP.append(i);
    }
  }

// qInfo() << "counter multiply" << counterMOrP.size();

  if (counterMOrP.size() == 0)
  {
    return string;
  }

  QString  stringFilter = "";  // without x (replace result)
  QString  secondNumber;
  int      indexOfNextOperation = getNextOperation(string, counterMOrP.at(0)); /// second number(index) between operation

// qInfo() << counterMOrP.at(0) + 1 << "D";

  for (int h = counterMOrP.at(0) + 1; h <= indexOfNextOperation; ++h)
  {
    secondNumber.append(string.at(h)); // second number

// qInfo() << string.at(h) << h << "D";
  }

// qInfo() << secondNumber;

  QString  firstNumber;
  int      indexOfPreviousOperation = getPreviousOperation(string, counterMOrP.at(0)); /// first number(index) between operation

  for (int h = indexOfPreviousOperation; h < counterMOrP.at(0); ++h)
  {
    firstNumber.append(string.at(h)); // first number
  }

  qInfo() << firstNumber << "first number in multiply";

  double  result = 0;

  if (string.at(counterMOrP.at(0)) == 'x')
  {
    if ((firstNumber == "") || (secondNumber == "") || (firstNumber == ".") || (secondNumber == "."))
    {
      return "Eror";
    }

    result = firstNumber.toDouble() * secondNumber.toDouble(); // multiply two number
    qInfo() << firstNumber << " * " << secondNumber << "=" << result;
  }
  else if (string.at(counterMOrP.at(0)) == '/')
  {
    if ((firstNumber == "") || (secondNumber == "") || (firstNumber == ".") || (secondNumber == "."))
    {
      return "Eror";
    }

    result = firstNumber.toDouble() / secondNumber.toDouble(); // devide two number
    qInfo() << firstNumber << " / " << secondNumber << "=" << result;

    if (QString::number(result) == "inf")
    {
      return "∞";
    }
  }

  for (int z = 0; z < string.size(); ++z)
  {
    if ((z >= indexOfPreviousOperation) && (z < indexOfNextOperation)) // add result too new string (stringFilter)
    {
    }
    else if (z == indexOfNextOperation)
    {
      stringFilter.append(QString::number(result));
    }
    else
    {
      stringFilter.append(string.at(z));
    }
  }

  qInfo() << stringFilter << "multiply result";

  if (counterMOrP.size() != 1)
  {
    return setMultiplyRanking(stringFilter); // do it for next nultiply
  }
  else
  {
    return stringFilter; // end of multiply in paranthese
  }
}

QString  ShowText::setSumRanking(QString string)
{
  QList<int>  counterSOrP;

  qInfo() << "string begin of sumRanking" << string;

  for (int i = 0; i < string.size(); ++i)
  {
    if ((string.at(i) == '+') || (string.at(i) == '-')) // how many sum or sub
    {
      if ((string.at(0) == '-'))
      {
        qInfo() << "bad happend" << "start negative problem";

        for (int z = 0; z < string.size(); ++z)
        {
          if (string.at(z) == '+')// z is sum index
          {
            int  startIndex = getPreviousOperation(string, z);
            int  endIndex   = getNextOperation(string, z);
            qInfo() << "string: " << string << "start Index: " << startIndex << "end Index: " << endIndex;
            QString  firstNumber;
            QString  secondNumber;

            for (int h = startIndex; h < z; ++h)
            {
              firstNumber.append(string.at(h));
            }

            for (int h = z + 1; h <= endIndex; ++h)
            {
              secondNumber.append(string.at(h));
            }

            double   result = firstNumber.toDouble() + secondNumber.toDouble();
            QString  newStr;

            for (int h = 0; h < startIndex; ++h)
            {
              newStr.append(string.at(h));
            }

            newStr.append(QString::number(result));

            for (int h = endIndex + 1; h < string.size(); ++h)
            {
              newStr.append(string.at(h));
            }

            qInfo() << "result of sum in negative problem" << newStr;

            return mResult(newStr);
          }
        }
      }
      else
      {
        counterSOrP.append(i);
      }
    }
  }

  if (counterSOrP.size() == 0)
  {
    return string;
  }

  QString  stringFilter = "";  // without x (replace result)
  QString  secondNumber;
  int      indexOfNextOperation = getNextOperation(string, counterSOrP.at(0)); /// first number(index) between operation

  for (int h = counterSOrP.at(0) + 1; h <= indexOfNextOperation; ++h)
  {
    secondNumber.append(string.at(h)); // first index
  }

  QString  firstNumber              = "";
  int      indexOfPreviousOperation = getPreviousOperation(string, counterSOrP.at(0)); /// second number(index) between operation

  for (int h = indexOfPreviousOperation; h < counterSOrP.at(0); ++h)
  {
    firstNumber.append(string.at(h)); // second number
  }

  double  result = 0;

  if (string.at(counterSOrP.at(0)) == '+')
  {
    if ((firstNumber == "") || (secondNumber == "") || (firstNumber == ".") || (secondNumber == "."))
    {
      return "Eror";
    }

    result = firstNumber.toDouble() + secondNumber.toDouble(); // sum
    qInfo() << firstNumber << " + " << secondNumber << "=" << result;
  }
  else if (string.at(counterSOrP.at(0)) == '-')
  {
    if ((firstNumber == "") || (secondNumber == "") || (firstNumber == ".") || (secondNumber == "."))
    {
      return "Eror";
    }

    result = firstNumber.toDouble() - secondNumber.toDouble(); // sub
    qInfo() << firstNumber << " - " << secondNumber << "=" << result;
  }

  for (int z = 0; z < string.size(); ++z) // add result too new string (stringFilter)
  {
    if ((z >= indexOfPreviousOperation) && (z < indexOfNextOperation))
    {
    }
    else if (z == indexOfNextOperation)
    {
      stringFilter.append(QString::number(result));
    }
    else
    {
      stringFilter.append(string.at(z));
    }
  }

  qInfo() << stringFilter << "3";

  if (counterSOrP.size() != 1)
  {
    return setSumRanking(stringFilter); // do it for next Operatiuon
  }
  else
  {
    return stringFilter; // end of sum
  }
}

int  ShowText::getNextOperation(QString string, int indexOperation)
{
  for (int i = indexOperation + 1; i < string.size(); ++i)
  {
    bool     ok          = true;
    QString  r           = QString(string.at(i));
    int      checkNumber = r.toInt(&ok);

    if (r == "-")
    {
      ok = true;
    }
    else if (r == ".")
    {
      ok = true;
    }
    else if (r == "\u00b2")
    {
      ok = true;
    }
    else if (ok == false)
    {
      return i - 1;
    }
  }

  return string.size() - 1;
}

int  ShowText::getNextOperationWithoutNegative(QString string, int indexOperation)
{
  for (int i = indexOperation + 1; i < string.size(); ++i)
  {
    bool     ok          = true;
    QString  r           = QString(string.at(i));
    int      checkNumber = r.toInt(&ok);

    if (r == ".")
    {
      ok = true;
    }
    else if (r == "-")
    {
      ok = true;
    }
    else if (ok == false)
    {
      return i - 1;
    }
  }

  return string.size() - 1;
}

bool  ShowText::isCorrect(QString string)
{
  bool  ok = true;

  for (int i = 0; i < string.size(); ++i)
  {
    QString  r = string.at(i);
    r.toInt(&ok);

    if (r == "-")
    {
      ok = true;
    }
    else if (r == ".")
    {
      ok = true;
    }
    else if (r == "\u00b2")
    {
      ok = true;
    }
    else if (r == "√")
    {
      ok = true;
    }
    else if (r == "x")
    {
      ok = true;
    }
    else if (r == "(")
    {
      ok = true;
    }
    else if (r == ")")
    {
      ok = true;
    }
    else if (r == "%")
    {
      ok = true;
    }
    else if (r == "+")
    {
      ok = true;
    }
    else if (r == "/")
    {
      ok = true;
    }

    if (ok == false)
    {
      return false;
    }
  }

  return true;
}

int  ShowText::getPreviousOperation(QString string, int indexOperation)
{
  bool  ok = true;

  for (int i = indexOperation - 1; i >= 0; --i)
  {
    QString  r = string.at(i);
    r.toInt(&ok);

    if (r == "-")
    {
      ok = true;
    }
    else if (r == ".")
    {
      ok = true;
    }
    else if (r == "\u00b2")
    {
      ok = true;
    }
    else if (ok == false)
    {
      return i + 1;
    }

    if (i == 0)
    {
      return 0;
    }
  }
}

int  ShowText::getPreviousOperationWithoutNegative(QString string, int indexOperation)
{
  bool  ok = true;

  for (int i = indexOperation - 1; i >= 0; --i)
  {
    QString  r = string.at(i);
    r.toInt(&ok);

    if (r == ".")
    {
      ok = true;
    }
    else if (r == "\u00b2")
    {
      ok = true;
    }
    else if (ok == false)
    {
      return i + 1;
    }

    if (i == 0)
    {
      return 0;
    }
  }
}

bool  ShowText::IsOperation(QString string, int indexOP)
{
  bool     ok    = true;
  QString  mChar = string.at(indexOP);

  mChar.toInt(&ok);

  if (mChar == '-')
  {
    ok = true;
  }
  else if (mChar == "\u00b2")
  {
    ok = true;
  }

  return ok;
}

bool  ShowText::IsOperationWithoutPower(QString string, int indexOP)
{
  bool     ok    = true;
  QString  mChar = string.at(indexOP);

  mChar.toInt(&ok);

  if (mChar == '-')
  {
    ok = true;
  }

  return ok;
}
