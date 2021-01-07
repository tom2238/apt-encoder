TEMPLATE = subdirs
SUBDIRS = image apt-encoder apt-colorm img-table aptcode

image.subdir = image
apt-encoder.subdir = apt-encoder
apt-colorm.subdir = apt-colorm
img-table.subdir = img-table
aptcode.subdir = aptcode

apt-encoder.depends = image aptcode
apt-colorm.depends = image aptcode
img-table.depends = image aptcode
image.depends = aptcode
