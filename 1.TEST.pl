#!/usr/bin/perl
# `rm -f out/*.dat`;
`rm -f my.out`;
$RADAR=$ARGV[0];
$YYYY=sprintf("%4d",$ARGV[1]);
$MM=sprintf("%02d",$ARGV[2]);

$y=$YYYY;
$ym=$MM;
@files=split(/\s+/,`ls data.src/$RADAR/$y/$YYYY$MM*rawacf.bz2`);
$mydate="$YYYY$MM"."01";
`cat hdw/hdw.dat.$RADAR | perl ./get_hdw_params.pl $mydate > hdw.$RADAR.$mydate`;

$beam=2;
foreach $f(@files)
{
$fn=$f; $fn=~ s/data\.src\/$RADAR\//data\.out\/$RADAR\//;
print "$f->$fn ($ymex)\n";
@datesplit=split(/[\/\.]/,$f);

print "S1:".(-s $fn).";\n";
print "S2:".(-s $f).";\n";
print "S3:".($f=~ /\/$ymex\./).";\n";

if(!(-s $fn) && !($f=~ /\/$ymex\./))
 {

   $dir=@datesplit[3];
   print "willmake: data.out/$RADAR/$dir/;\n";
  `mkdir -p data.out/$RADAR/$dir/`;
  $fn2=$fn; $fn2=~ s/\.bz2//g;
print "process it $fn2\n";
  
 if(
 1==1
 )
 {
  print "processing ".$f."...\n";
  `bunzip2 -c $f >tmp.$RADAR.rawacf`;

   $date_pr=@datesplit[4];
   $f2=$f; $f2=~ s/\.bz2$//;
   print "date: $date_pr $fn2\n";
  print "./recalc_elevation_any_radar tmp.$RADAR.fitacf $fn2 phases.corrected.$RADAR >/dev/null 2>>/tmp/log.$RADAR.recalcV3.err\n";
  `./main_raw_refit tmp.$RADAR.rawacf hdw.$RADAR.$mydate cal/$RADAR.cal 1>>my.$RADAR.out 2>log.$RADAR.recalcV3.err`;
   print "mv tmp.$RADAR.rawacf.cbor $fn2.aampacf\n";
  `mv tmp.$RADAR.rawacf.cbor $fn2.aampacf`;
  `bzip2 $fn2.aampacf`;
 
 }
 }
}
