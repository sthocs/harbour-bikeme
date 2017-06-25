.pragma library

function makeLastUpdateDateHumanReadable(update_date) {
    if (isNaN(update_date)) {
        return "N/A";
    }
    var now = new Date();
    var elapsedSeconds = (now.getTime() - update_date.getTime()) / 1000;
    var min = Math.floor(elapsedSeconds / 60);
    var sec = Math.floor(elapsedSeconds % 60);
    return min + " min " + sec
}

function isValidDate(date) {
    return !isNaN(date);
}
