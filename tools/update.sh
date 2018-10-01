#!/bin/bash

curl ftp://ftp.modland.com/pub/documents/allmods_md5.zip -o allmods_md5.zip
unzip allmods_md5.zip && rm allmods_md5.zip
sed -e 's/^[0-9a-f]* //g' allmods_md5.txt | sort > allmods-sorted.txt

supported_tracker=0
tracker="none"
author=
module=

if [ ! -e utwory.sqlite ]
then
	echo "
	CREATE TABLE authors (id integer primary key, title text);
	CREATE TABLE songs (id integer primary key, filename text, title text);
	CREATE TABLE song_author_map (id integer primary key, author_id integer, song_id integer);
	" | sqlite3 utwory.sqlite
fi

# to make things easier just grab whatever is in one of supported formats
# will cut list a lot
while read tracker
do
	grep $tracker allmods-sorted.txt >>allmods-supported.txt
done < trackers-supported.txt

while read module
do
	module_tracker=$(echo "$module" | cut -d'/' -f1)
	module_author=$( echo "$module" | cut -d'/' -f2)

	TMP=$(mktemp -u)

	if [ "- unknown" == "$module_author" ]
	then
		continue
	fi

	if [ "$tracker" != "$module_tracker" ] 
	then
		echo -n "Checking support for $module_tracker... "

		skip_checks=0
		supported_tracker=0

		grep "$module_tracker" trackers-supported.txt >/dev/null
		if [ 0 == $? ]
		then
			echo 'already supported'
			skip_checks=1
			supported_tracker=1
		else
			grep "$module_tracker" trackers-unsupported.txt >/dev/null
			if [ $? == 0 ]
			then
				echo 'already not supported'
				skip_checks=1
			fi
		fi

		if [ 0 == $skip_checks ]
		then
			curl "ftp://ftp.modland.com:/pub/modules/$module" -o $TMP -s
			./test $TMP
			if [ $? == 0 ]
			then
				echo 'supported'
				echo $module_tracker >>trackers-supported.txt
				supported_tracker=1
			else
				echo 'not supported'
				echo $module_tracker >>trackers-unsupported.txt
			fi
		fi

		tracker=$module_tracker
	fi

	if [ 1 == $supported_tracker ]
	then
		if [ "$author" != "$module_author" ]
		then
			author_id=$(echo "select id from authors where title='$module_author' limit 1"|sqlite3 utwory.sqlite)

			if [ ! $author_id ]
			then
				author_id=$(echo "insert into authors values (null, '$module_author'); select last_insert_rowid();"|sqlite3 utwory.sqlite)
			fi
		fi

		song_id=$(echo 'insert into songs values (null, "'$module'", "'$(basename "$module")'");select last_insert_rowid();'| sqlite3 utwory.sqlite)
		echo "insert into song_author_map values (null, $author_id, $song_id)" |sqlite3 utwory.sqlite
		echo "added song $song_id"
	fi

	author=$module_author

# to update list of (un)supported trackers use allmods-sorted.txt in below line
done <allmods-supported.txt

sort -u trackers-supported.txt >$TMP;mv $TMP trackers-supported.txt
sort -u trackers-unsupported.txt >$TMP;mv $TMP trackers-unsupported.txt
