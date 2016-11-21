
function doGrading() {


var name = $('.ui-selectmenu-item-header')[0].innerHTML.replace('-','')
var subName = name.split(' ')[1].toLowerCase()+name.split(' ')[0].toLowerCase();
var scoreData;

$.get('//localhost:4443/results/'+subName+'/grade_report.txt', function(data) { 
scoreData = data; 

var commentArea=$('#speedgrader_comment_textarea');
commentArea.val(scoreData);

var submitBtn = $('#comment_submit_button');
submitBtn.click();

var rubricBtn = $('.toggle_full_rubric.edit.btn');
rubricBtn.click();

var meGrader = $('#rating_970_1872');
meGrader.click();

var fullMarks = $('#rating_970_960');
fullMarks.click();

var goodStyle = $('#rating_970_9715');
goodStyle.click();

var score = scoreData.split('Total: ')[1].split(' / ')[0];

var points = $('td.nobr.points_form > div.editing > span > input').first();
points.val(parseInt(score)+5);

var saveRubricBtn = $('.save_rubric_button');
saveRubricBtn.focus();

});

var saveRubricBtn = $('.save_rubric_button');
saveRubricBtn.focus();

clear(); // chrome / FF console clearing

var saveRubricBtn = $('.save_rubric_button');
saveRubricBtn.focus();

subName;

}

// Fill in additional comments instead of assignment comment
$resultsComment=$('.no-hover.criterion_comments_link')[0];
$resultsComment.click();
$resultsCommentTA = $('#criterion_comments_textarea');
$resultsCommentTA.val(scoreData);

$subBtn = $('#rubric_criterion_comments_dialog div.editing > div.button-container > .btn.save_button');

$subBtn.click();







