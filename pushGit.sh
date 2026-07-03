message="$1"

echo "You are pushing changes to sky-os main. Continue? (Y/n): "
read in

if [[ "$in" == "n" || "$in" == "N" ]]; then
    exit 1
fi

git add .
git commit -m "$message"
git push origin main