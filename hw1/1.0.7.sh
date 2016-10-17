numFiles="0"
numExecutable="0"
dirs=""
for f in ~/*; do
	numFiles="$(($numFiles + 1))"
	if [[ -x "$f" ]]; then
		numExecutable="$(($numExecutable + 1))"
	fi
	if [[ -d "$f" ]]; then
		size=`du -sh "$f" | cut -f1`
		dirs="$dirs\n$f $size"
	fi
done
echo "Total number of files: $numFiles"
echo "Number of executable files: $numExecutable"
echo -e "Sizes of each directory: $dirs"
