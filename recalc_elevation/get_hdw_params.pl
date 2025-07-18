use Time::Local;

$select_date=$ARGV[0];
$Y=substr($select_date,0,4);
$M=substr($select_date,4,2);
$D=substr($select_date,6,2);
while ($s=<STDIN>)
{
 @d=split(/\s+/,$s);
 if (!($s=~ /^#/) && @d[1] && @d[14]!=0)
  {
   @p=split(/\s+/,$s);
#   print "cmp: ".@p[2]." $Y\n";
   if (@p[2]<=$Y || !$last)
    {
     $dt=timegm(0,0,0,$D,$M-1,$Y-1900)-timegm(0,0,0,1,0,$Y-1900);
     if(@p[2]==$Y && @p[3]>$dt) #+24*3600)
     {
     }
     else
     {
      $last=$s; 
#      print "save!\n";
     }
    }
  }
}
@d=split(/\s+/,$last);
print "".@d[4]." ".@d[5]." ".@d[7]." ".@d[8]." ".(@d[14])." ".@d[15]." ".@d[19]." ".@d[12]."\n";