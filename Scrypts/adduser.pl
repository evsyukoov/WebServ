#!/usr/bin/perl

#cgi lib, no need to parse form
use strict;
use CGI qw(:standard);

#Get a CGI object
my $cgiform = new CGI;

#Extract username and password
my $username = $cgiform->param("username");  #Gets values from form
my $password = $cgiform->param("password");

#Output the username and password
   print header();
   print start_html("Add a User");
   print "<h2>Username: $username</h2>\n";
   print "<h2>Password: $password</h2>\n";
   print end_html();