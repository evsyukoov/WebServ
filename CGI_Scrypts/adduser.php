#! /usr/bin/php

<html>
<body>

<?php
   // File:  adduser.php
   // Place in your cgi-bin directory and make executable.
   // remove the spaces around html and body, /html and /body, and ?php


echo "<h2>Username: " . $_ENV["AUTH_TYPE"] . "</h2></br>";
echo "<h2>Password: " . $_ENV["AUTH_TYPE"] . "</h2></br>";
?>

</body>
</html>