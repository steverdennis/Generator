#!/usr/bin/perl
#---------------------------------------------------------------------
# Given a directory, the script looks for file in the form 
# v*_on_*_*.xml 
# then it calls the appropriate gspladd to obtain v*_on_*.xml comprehensive
# file. The file name will be total_xsec.xml
# The scpript also prodces a root output for the splines
# 
#  Options:
#  [--dir]             : Is the directory which contains all the xml files to be converted. Default: $PWD
#  [--tune]            : Tune option
#  [--add-list]        : additional file list to be included when the total_xsec.xml file is created
#  [--root-output]     : Create an output file with all the splines
#  [--def-nu-list]     : Default list of neutrino PDG to be used if that cannot be derived from local files. Default 14
#  [--evet-gen-list]   : Event generator list used in the root file output
#  [--add-nucleons]    : When the ROOT file is created, also splines for proton and neutrons are created
#  [--save-space]      : remove intermadiate xml files
#  
#---------------------------------------------------------------------

$iarg=0;
foreach (@ARGV) {
  if($_ eq '--dir')             { $dir            = $ARGV[$iarg+1]; }
  if($_ eq '--tune')            { $tune           = $ARGV[$iarg+1]; }
  if($_ eq '--add-list')        { $add_list       = $ARGV[$iarg+1]; }
  if($_ eq '--root-output')     { $root_output    = 1 ; }
  if($_ eq '--event-gen-list')  { $event_gen_list = $ARGV[$iarg+1]; }
  if($_ eq '--def-lepton-list') { $def_lepton_list    = $ARGV[$iarg+1]; }
  if($_ eq '--add-nucleons')    { $add_nucleons   = 1 ; }
  if($_ eq '--save-space' )     { $save_space     = 1 ; } 
  $iarg++;
}

$dir=$ENV{'PWD'}      unless defined $dir;
$def_lepton_list = "14"   unless defined $def_lepton_list ;

opendir(DIR, $dir) or die $!;

%leptons = ();
%tags = ();
%lists = ();

while (my $file = readdir(DIR) ) {

  # We only want files
  next unless (-f "$dir/$file");

  # Use a regular expression to find files ending in .xml
  next unless ($file =~ m/\.xml$/);

  next unless ( index($file, "_on_") != -1 );

  my $under_counter = $file =~ tr/_//; 
  next unless ( $under_counter == 3 );

  my $lepton = substr($file, 0, index($file, '_') );
  if ( ! exists( $leptons{$lepton} ) ) { $leptons{$lepton}=1; } 

  my $tgt = substr($file, index($file, "on_")+3, rindex($file, '_')-index($file, "on_")-3 );
  if ( ! exists( $tgts{$tgt} ) ) { $tgts{$tgt}=1; } 

  my $list = substr($file, rindex($file, '_')+1, index($file,'.')-rindex($file, '_')-1 );
  if ( ! exists( $lists{$list} ) ) { $lists{$list}=1; } 

  print "$file \n";

}

closedir(DIR);

print " Leptons: \n";
foreach my $lepton ( keys %leptons ) {
    print "$lepton ";
}

print "\n targets: \n";
foreach my $tgt ( keys %tgts ) {
    print "$tgt ";
}

print "\n Processes: \n";
foreach my $proc ( keys %lists ) {
    print "$proc ";
}
print "\n";

$leptons{'e'}=11        if ( exists( $leptons{'e'} ) );
$leptons{'ebar'}=-11    if ( exists( $leptons{'ebar'} ) );
$leptons{'mu'}=13       if ( exists( $leptons{'mu'} ) );
$leptons{'mubar'}=-13   if ( exists( $leptons{'mubar'} ) );
$leptons{'tau'}=15      if ( exists( $leptons{'tau'} ) );
$leptons{'taubar'}=-15  if ( exists( $leptons{'taubar'} ) );
$leptons{'ve'}=12        if ( exists( $leptons{'ve'} ) );
$leptons{'vebar'}=-12    if ( exists( $leptons{'vebar'} ) );
$leptons{'vmu'}=14       if ( exists( $leptons{'vmu'} ) );
$leptons{'vmubar'}=-14   if ( exists( $leptons{'vmubar'} ) );
$leptons{'vtau'}=16      if ( exists( $leptons{'vtau'} ) );
$leptons{'vtaubar'}=-16  if ( exists( $leptons{'vtaubar'} ) );

$lepton_list="";
foreach $lepton ( keys %leptons ) {
    $lepton_list .= "," if ( $lepton_list ne "" );
    $lepton_list .= $leptons{$lepton};
}
##print $lepton_list."\n";

$proc_list = "";
foreach $proc ( keys %lists ) {
  $proc_list .= "," if ($proc_list ne "" );
  $proc_list .= $proc ; 
} 

$tgt_file_list = "";
$tgt_list = "";

foreach my $tgt ( keys %tgts ) {

  $tmp_tgt;
  if ( length($tgt) == 1 ) {
      $tmp_tgt = 1000010010 if ( $tgt eq 'p' );
      $tmp_tgt = 1000000010 if ( $tgt eq 'n' );
  }   
  else { $tmp_tgt = $tgt; }

  $lepton_file_list = "";

  foreach my $lepton ( keys %leptons ) {
  
    $proc_file_list = "";
    
    foreach my $proc ( keys %lists ) {

      $tmp_proc_file="$dir/".$lepton."_on_".$tgt."_".$proc.".xml";
      if ( -f $tmp_proc_file ) {
	  $proc_file_list .= "," if ( $proc_file_list ne "" );
          $proc_file_list .= "$tmp_proc_file";
      }
      else {
	  print "Warning: Missing file $tmp_proc_file \n";
      }
    }  ##proc list

    $tmp_lepton_file = "$dir/".$lepton."_on_".$tgt.".xml"; 

    $gspladd_opt    = " -o $tmp_lepton_file -f $proc_file_list ";
    $gspladd_cmd    = "gspladd $gspladd_opt";
    print "$gspladd_cmd \n \n";
    `$gspladd_cmd \n`;

    if ( -f $tmp_lepton_file ) {
	$lepton_file_list .= "," if ( $lepton_file_list ne "" );
        $lepton_file_list .= "$tmp_lepton_file";
    }
    else { 
	print "Error: $tmp_lepton_file not created \n";
    }
    
  } ## lepton loop 

  $tmp_tgt_file = "$dir/".$tgt.".xml"; 

  $leptons_size = keys %leptons ;

  if ( $leptons_size > 1 ) { 
    $gspladd_opt    = " -o $tmp_tgt_file -f $lepton_file_list ";
    $gspladd_cmd    = "gspladd $gspladd_opt";
    print "$gspladd_cmd \n \n";
    `$gspladd_cmd \n`;
  }
  elsif ( $leptons_size == 1 ) {
    $cp_cmd = "cp $lepton_file_list $tmp_tgt_file ";
    print "$cp_cmd \n \n";
    `$cp_cmd \n`;  
  }
  elsif ( $leptons_size == 0 ) {
    print "\nError: No Neutrino flavours \n";
  }

  if ( -f $tmp_tgt_file ) {
    $tgt_file_list .= "," if ( $tgt_file_list ne "" );
    $tgt_file_list .= "$tmp_tgt_file";
    $tgt_list .= "," if ( $tgt_list ne "" );
    $tgt_list .= "$tmp_tgt";
  }
  else { 
    print "\nError: $tmp_tgt_file not created \n";
  }
  
}  ##tgt loop

$glob_file = $dir."/total_xsec.xml"; 

$tgt_size = keys %tgts;
if ( ($tgt_size > 1) or (defined $add_list ) ) {

  if ( defined $add_list ) {
    $tgt_file_list = "$add_list" . "," . "$tgt_file_list" ;
  }

  if ( defined $add_leptoncleons ) {
    if ( index($tgt_list, "1000010010") == -1 ) {
      $tgt_list .= ",1000010010";
    } 

    if ( index($tgt_list, "1000000010") == -1 ) {
      $tgt_list .= ",1000000010";
    }
  }

  ##$grep_pipe     = "grep -B 100 -A 30 -i \"warn\\|error\\|fatal\"";
  $gspladd_opt    = " -o $glob_file -f $tgt_file_list ";
  $gspladd_cmd    = "gspladd $gspladd_opt";
  print "$gspladd_cmd \n \n";
  `$gspladd_cmd \n`;
}
elsif ( $tgt_size == 1 ) {

  ###if there is only one target, the global file has to be created manually
  my $temp_name = "$dir/".(keys %tgts)[0].".xml";
  
  my $cmd = "ln -s "; 
  $cmd = "cp " if defined $save_space ;  

  $cmd .= " $temp_name $glob_file ";
  print "Executing: $cmd \n" ; 
  `$cmd` ;
}

if ( defined $root_output ) {

##
## Create an output file with all the splines in root format
##
    
  if ( $lepton_list eq "" ) {
    $lepton_list = $def_lepton_list ;
  }

  if ( index($tgt_list, "1000010010") == -1 ) {
    $tgt_list .= "," unless ( $tgt_list eq "" ) ;
    $tgt_list .= "1000010010" ;
  }

  if ( index($tgt_list, "1000000010") == -1 ) {
    $tgt_list .= "," unless ( $tgt_list eq "" ) ;
    $tgt_list .= "1000000010";
  }

  my $cmd = "gspl2root ";
  $cmd .= " -p $lepton_list ";
  $cmd .= " -t $tgt_list ";
  $cmd .= " -f $glob_file ";
  $cmd .= " -o $dir"."/total_xsec.root " ;
  if ( defined $event_gen_list ) {
    $cmd .= " --event-generator-list $event_gen_list " 
  }
  $cmd .= " --tune $tune " if ( defined $tune ) ;
  print "\n$cmd\n";
  `$cmd`;

}


if ( defined $save_space ) { 

##
## removing intermediate xml file 
##

  foreach my $tgt ( keys %tgts ) {

    foreach my $lepton ( keys %leptons ) {
  
      my $tmp_lepton_file = "$dir/".$lepton."_on_".$tgt.".xml"; 

      if ( -f $tmp_lepton_file ) {
        `rm $tmp_lepton_file` ;
      }
    
    } ## lepton loop 

    my $tmp_tgt_file = "$dir/".$tgt.".xml"; 

    if ( -f $tmp_tgt_file ) {
      `rm $tmp_tgt_file` ;
    }
  
  }  ##tgt loop

}  ## if defined $save_space 




