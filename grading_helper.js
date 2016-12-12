
function doGrading() {

var name = $('.ui-selectmenu-item-header')[0].innerHTML.replace('-','')
var subName = name.split(' ')[0].toLowerCase()+name.split(' ')[1].toLowerCase();
var scoreData;

$.get('//localhost:4443/results/'+subName+'/grade_report.txt', function(data) { 
scoreData = data+"\n<br/>Grader:adbhat@cs"; 

var rubricBtn = $('.toggle_full_rubric.edit.btn');
rubricBtn.click();

var fullSubmission = $('#rating_blank');
fullSubmission.click();

var fullHeaders = $('#rating_1646_1705');
fullHeaders.click();

var fullMarks = $('#rating_1646_1917');
fullMarks.click();

var goodStyle = $('#rating_1646_5500');
goodStyle.click();

var score = scoreData.split('Total points: ')[1].split('\n')[0];

var points = $('#criterion_1646_6549 > td.nobr.points_form > div.editing > span > input');
points.val(parseInt(score));

// Fill in additional comments instead of assignment comment
$resultsComment=$('.no-hover.criterion_comments_link')[3];
$resultsComment.click();
$resultsCommentTA = $('#criterion_comments_textarea');
$resultsCommentTA.val(scoreData);

$subBtn = $('#rubric_criterion_comments_dialog div.editing > div.button-container > .btn.save_button');
$subBtn.click();

var saveRubricBtn = $('#rubric_holder > div.button-container > button.save_rubric_button.Button.Button--primary');
saveRubricBtn.focus();

});

$.get('//localhost:4443/students/'+subName+'/comments.txt', function(data) {
	alert(data);
});

window.open("//localhost:4443/students/"+subName+"/IndexTree.java");

clear(); // chrome / FF console clearing

subName;

}



#var commentArea=$('#speedgrader_comment_textarea');
#commentArea.val(scoreData);

#var submitBtn = $('#comment_submit_button');
#submitBtn.click();

#var meGrader = $('#rating_970_1872');
#meGrader.click();




