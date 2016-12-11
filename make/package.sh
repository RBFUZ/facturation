#!/bin/bash

endScriptNormal() {
    qdbus $dbusRef close 2> /dev/null > /dev/null
    exit 0
}

endScriptErrorBox() {
    kdialog --title "$1" --error "$2"
    qdbus $dbusRef close 2> /dev/null > /dev/null
    exit 1
}

endScriptError() {
    qdbus $dbusRef close 2> /dev/null > /dev/null
    exit 1
}



if [[ -z "$(which kdialog)" ]]; then
    echo "kdialog not found."
    exit 1
fi
if [[ -z "$(which curl)" ]]; then
    echo "curl not found." 
    exit 1
fi

if [[ -z "$(which diff)" ]]; then
    echo "diff not found." 
    exit 1
fi


dbusRef=$(kdialog --geometry 600x100 --title "Preparing an archive for the teacher" --progressbar "Initializing...                                                                                                                                       " 11)
rm -rf archive 2> /dev/null > /dev/null
mkdir -p archive archive/src archive/debug archive/release


# Step 0 : update source code
#qdbus $dbusRef setLabelText "Updating source code..." 2> /dev/null > /dev/null
#qdbus $dbusRef Set "" value 1 2> /dev/null > /dev/null
#./update.sh || endScriptNormal


# Step 1 : source code validity
qdbus $dbusRef setLabelText "Checking sources validity..." 2> /dev/null > /dev/null
qdbus $dbusRef Set "" value 1 2> /dev/null > /dev/null
if [[ -n $(grep user_ src/* | grep -v data) ]]; then 
    endScriptErrorBox "Checking sources validity..." "Using user_XXX functions in your code is forbidden. You must correct it to continue."
fi


# Step 2 : source code compilation
qdbus $dbusRef setLabelText "Compiling sources..." 2> /dev/null > /dev/null
qdbus $dbusRef Set "" value 2 2> /dev/null > /dev/null
make clean all 2>&1 | tee archive/compile.log || endScriptErrorBox "Compiling sources..." "Compilation failed. See archive/compile.log for details" 

# Step 3 : student details
qdbus $dbusRef setLabelText "Checking student details..." 2> /dev/null > /dev/null
qdbus $dbusRef Set "" value 3 2> /dev/null > /dev/null

studentfirstname=$(debug/facturation disable-dump-usage student-firstname | tr -cd '[a-zA-Z \-]') || endScriptErrorBox "Checking student details..." "An error occurred while gathering student details"
studentlastname=$(debug/facturation disable-dump-usage student-lastname | tr -cd '[a-zA-Z \-]') || endScriptErrorBox "Checking student details..." "An error occurred while gathering student details"
studentyears=$(debug/facturation disable-dump-usage student-years | tr -cd '[0-9\\-]') || endScriptErrorBox "Checking student details..." "An error occurred while gathering student details"
studentgroup=$(debug/facturation disable-dump-usage student-group | tr -cd '[a-zA-Z 0-9\\-]') || endScriptErrorBox "Checking student details..." "An error occurred while gathering student details"
kdialog --title "Checking student details..." \
	--yes-label "I confirm this is correct" \
	--no-label "There are errors" \
	--yesno "Your details (invalid characters have been removed) :\n-> Your first name : $studentfirstname\n-> Your last name : $studentlastname\n-> Your years : $studentyears\n-> Your group : $studentgroup" \
	|| endScriptError

# Step 4 : copy sources files
qdbus $dbusRef setLabelText "Copying sources code..." 2> /dev/null > /dev/null
qdbus $dbusRef Set "" value 4 2> /dev/null > /dev/null
cp src/main.c archive/src/
cp src/CatalogDB.c archive/src/
cp src/CatalogRecord.c archive/src/
cp src/CustomerDB.c archive/src/
cp src/CustomerRecord.c archive/src/
cp src/Dictionary.c archive/src/
cp src/Document.c archive/src/
cp src/DocumentRowList.c archive/src/
cp src/DocumentUtil.c archive/src/
cp src/EncryptDecrypt.c archive/src/
cp src/MyString.c archive/src/
cp src/OperatorTable.c archive/src/
cp src/PrintFormat.c archive/src/


# Step 5 : detect not implemented functions
qdbus $dbusRef setLabelText "Detecting not implemented functions..." 2> /dev/null > /dev/null
qdbus $dbusRef Set "" value 5 2> /dev/null > /dev/null

cat $(grep IMPLEMENT archive/src/* | cut -f1 -d ':') \
    | sed 's/\(.*\)IMPLEMENT(\(.*\))(/\1user_\2(/g' \
    | sed "s/UNUSED(\([^)]*\))/\1/g" \
    | java -Dfile.encoding=UTF-8 -jar make/analyzer-simple.jar \
    | sort | uniq \
    | while read line; do 
        fct=$(echo $line | cut -f3 -d '#' | sed "s/user_//g")
        if [[ -n $(echo $line | grep "provided_$fct") ]]; then
            echo "$fct => NOT implemented";
        fi
    done | tee archive/notimplemented.txt
kdialog --title "List of not implemented functions" --yes-label "I confirm this list of not implemented functions" --no-label "I need to modify my source code" --yesno "$(<archive/notimplemented.txt)" || endScriptError

# Step 6&7 : valgring
qdbus $dbusRef setLabelText "Running valgrind on the debug executables..." 2> /dev/null > /dev/null
qdbus $dbusRef Set "" value 6 2> /dev/null > /dev/null
make valgrind-debug-nogui | tee archive/valgrind-debug.txt 

qdbus $dbusRef setLabelText "Running valgrind on the release executables..." 2> /dev/null > /dev/null
qdbus $dbusRef Set "" value 7 2> /dev/null > /dev/null
make valgrind-release-nogui | tee archive/valgrind-release.txt 

# Step 8 : copy executables
qdbus $dbusRef setLabelText "Copying executables..." 2> /dev/null > /dev/null
qdbus $dbusRef Set "" value 8 2> /dev/null > /dev/null
cp debug/facturation archive/debug/
cp release/facturation archive/release/

# Step 9 : build archive
qdbus $dbusRef setLabelText "Building the archive..." 2> /dev/null > /dev/null
qdbus $dbusRef Set "" value 9 2> /dev/null > /dev/null

archive="$(debug/facturation disable-dump-usage student-lastname | tr -cd '[a-zA-Z \-]' | tr ' ' '_')@$(debug/facturation disable-dump-usage student-firstname | tr -cd '[a-zA-Z \-]' | tr ' ' '_').tar.gz"
rm $archive 2> /dev/null > /dev/null 
tar -czf $archive archive/ 2>&1 | tee archive.log || endScriptErrorBox "Building the archive..." "An error occurred while building the archive (see archive.log for details)."

kdialog --title "Verify the content of the archive..." --yes-label "All is in the archive" -no-label "Something is missing" --yesno "$(tar -tf $archive)" || ensScriptError


# Step 10 : archive upload
qdbus $dbusRef setLabelText "Uploading the archive..." 2> /dev/null > /dev/null
qdbus $dbusRef Set "" value 10 2> /dev/null > /dev/null


mail=$(kdialog --title "What is your email ?" --inputbox "What is your email ?" "")

kdialog --title "Upload" --yesno "Did you already received a secret id by mail ?" --yes-label "yes" --no-label "no"
if [[ "$?" != "0" ]]; then
    curl "https://storage.projectsforge.org/requestid.php?mail=$mail&project=c"
    kdialog -msgbox "You should receive soon your secret id by mail."
fi

id=$(kdialog --title "What is your secret id ?" --inputbox "What is your secret id ?" )

curl -F"operation=upload" -F"file=@$archive" "https://storage.projectsforge.org/upload.php?mail=$mail&project=c&id=$id" -o downloaded.tar.gz

diff $archive downloaded.tar.gz && kdialog --title "Upload result" --msgbox "Your archive was uploaded. Bye." || endScriptErrorBox "Error" "An error occurred while uploading your archive."

endScriptNormal
