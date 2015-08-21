#!/bin/perl

############################################################################
#
# Converts GCC error messages to error messages understandable by VS6
#
############################################################################

############################################################################
# Perl trim function to remove whitespace from the start and end of the string
############################################################################
sub trim($)
{
	my $string = shift;
	$string =~ s/^\s+//;
	$string =~ s/\s+$//;
	return $string;
}
 
#foreach my $line (@lines)
while (defined($line = <STDIN>))
{
	$line = trim($line);
	
	if($line =~ /error:/)
	{
			$line =~ s/:([0-9]+):/($1) :/;
	}
	if($line =~ /expects/)
	{
			$line =~ s/:([0-9]+):/($1) :/;
	}
	if($line =~ /warning:/)
	{
			$line =~ s/:([0-9]+):/($1) :/;
	}
	
	print $line . "\n";
}


























