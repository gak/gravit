cd ..
make doc
cd www
scp gravit.css index.php slowchop@slowchop.com:www/gravit/
scp docs/*.php slowchop@slowchop.com:www/gravit/docs/
