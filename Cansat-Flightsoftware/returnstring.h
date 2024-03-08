String addZeroSuffix(String st, int n)
{
  for(int i=0;i<n;i++)
    st = st + "0";
  
  return st;
}

String addZeroToPrefix(String st, int n)
{
  for(int i=1;i<=n;i++)
    st = "0" + st;

  return st;
}

String printFloat(double str, int len, int lenAfterDecimal, bool valid)
{
  String finalStri="";
  bool neg = false;
  if (valid)
  {
    if ( str < 0  ){
      neg = true;
      str *= -1;
      len--;
    }
    String s = String(str,len);
    int indexOfPt = s.indexOf(".");
    if((len)-(indexOfPt+1) >= lenAfterDecimal)
      finalStri = s.substring(0, (indexOfPt+1+lenAfterDecimal));
    else
    {
      int zeroToSuffix = lenAfterDecimal - (len - (indexOfPt+1));
      finalStri = addZeroSuffix(s, zeroToSuffix);
    }
    int lenBeforeDecimal = len - (lenAfterDecimal + 1);

    if(lenBeforeDecimal >= indexOfPt)
    {
      int zeroToPrefix = lenBeforeDecimal - indexOfPt;
      finalStri = addZeroToPrefix(finalStri, zeroToPrefix);
    }
    if ( neg ){
      finalStri = "-" + finalStri;
    }
    return finalStri;
  }
  else
  {
    for (int i=0; i<len; i++)
      finalStri+="*";
  }
  return finalStri;
}

 String printInt(unsigned long val, int len, bool valid)
{
  String finalStr="";
  if (valid)
  {
    String st = String(val);
    int zeroAtPrefix = len - (st.length());
    finalStr = addZeroToPrefix(st, zeroAtPrefix);
    return finalStr;
  }

  else
  {
    for (int i=0; i<len; i++)
      finalStr+="*";
  }
  return(finalStr);
}



String printStr(const char *str, int len, bool valid)
{

    String stri="";
    if (valid)
    {
      int slen = strlen(str);
      for (int i=0; i<len; ++i)
          if (i<slen)
              stri+=str[i];
          else
              stri+=" ";
    }
    else
    {
      for (int i=0; i<len; ++i)
        stri+="*";
    }
    return stri;

}

String printDate(int day, int month, int year, bool valid)
{
    String date="";
    if (valid)
    {
        String d = String(day);
        String m = String(month);
        String yr = String(year);

         if(day/10 == 0)
        {
          d = "0" + d;
        }

        if (month/10==0)
        {
          m = "0" + m;
        }
        if (year/10==0)
        {
          yr = "0" + yr;
        }

        date = yr + ":" + m + ":" + d;
    }
    else
    {
        date="**********" ;  
    }
    return date;
}

String printTime(int hour, int minute, int second, bool valid)
{
    String tim="";
    if (valid)
    {
        String hr = String(hour);
        String min = String(minute);
        String sec = String(second);
        if(hour/10 == 0)
        {
          hr = "0" + hr;
        }

        if (minute/10==0)
        {
          min = "0" + min;
        }
        if (second/10==0)
        {
          sec = "0" + sec;
        }

        tim = hr + ":" + min + ":" +sec;
    }
    else
    {
        tim="********" ;  
    }
    return tim;
}
