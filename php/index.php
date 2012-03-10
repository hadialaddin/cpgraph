<?php
	session_start();
	include("include/RestRequest.inc.php");
	include("include/config.inc.php");
	
	if(isset($_SESSION['cp_access_token'])){
		$cp_request = new RestRequest('http://graph.coursepeer.com/index.php/oauth/verify_access_token?access_token='.$_SESSION['cp_access_token'],'POST');
		$cp_request->buildPostBody(array());
		$cp_request->execute();
		$result = $cp_request->getResponseBody();
		if($result['type']=='external'){
			// access_token found
			$cp_loggedin = 1;
		}
		else{
			// access_token not found
			$cp_loggedin = 0;
		}
	}
	else{
		$cp_loggedin = 0;
	}
?>

<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <title>CoursePeer Sample Application</title>
    <meta name="description" content="">
    <meta name="author" content="">

    <!-- Le HTML5 shim, for IE6-8 support of HTML elements -->
    <!--[if lt IE 9]>
      <script src="http://html5shim.googlecode.com/svn/trunk/html5.js"></script>
    <![endif]-->

    <!-- Le styles -->
    <link href="http://twitter.github.com/bootstrap/1.4.0/bootstrap.min.css" rel="stylesheet">
	<script src="http://ajax.googleapis.com/ajax/libs/jquery/1.7.0/jquery.min.js"></script>
	<script src="http://twitter.github.com/bootstrap/1.4.0/bootstrap-twipsy.js"></script>
	<script src="http://twitter.github.com/bootstrap/1.4.0/bootstrap-popover.js"></script>
	<script src="http://twitter.github.com/bootstrap/1.4.0/bootstrap-alerts.js"></script>
	<script>
		$(function () {
			$(".alert-message").alert();
		})
	</script>
	
    <style type="text/css">
      /* Override some defaults */
      html, body {
        background-color: #eee;
      }
      body {
        padding-top: 40px; /* 40px to make the container go all the way to the bottom of the topbar */
      }
      .container > footer p {
        text-align: center; /* center align it with the container */
      }
      .container {
        width: 820px; /* downsize our container to make the content feel a bit tighter and more cohesive. NOTE: this removes two full columns from the grid, meaning you only go to 14 columns and not 16. */
      }

      /* The white background content wrapper */
      .container > .content {
        background-color: #fff;
        padding: 20px;
        margin: 0 -20px; /* negative indent the amount of the padding to maintain the grid system */
        -webkit-border-radius: 0 0 6px 6px;
           -moz-border-radius: 0 0 6px 6px;
                border-radius: 0 0 6px 6px;
        -webkit-box-shadow: 0 1px 2px rgba(0,0,0,.15);
           -moz-box-shadow: 0 1px 2px rgba(0,0,0,.15);
                box-shadow: 0 1px 2px rgba(0,0,0,.15);
      }

      /* Page header tweaks */
      .page-header {
        background-color: #f5f5f5;
        padding: 20px 20px 10px;
        margin: -20px -20px 20px;
      }

      /* Styles you shouldn't keep as they are for displaying this base example only */
      .content .span10,
      .content .span4 {
        min-height: 500px;
      }
      /* Give a quick and non-cross-browser friendly divider */
      .content .span4 {
        margin-left: 0;
        padding-left: 19px;
        border-left: 1px solid #eee;
      }

      .topbar .btn {
        border: 0;
      }

    </style>

  </head>

  <body>

    <div class="topbar">
      <div class="fill">
        <div class="container">
          <a class="brand" href="./">CoursePeer</a>
          <div class="pull-right">
			<?php if($cp_loggedin==1){ ?><a href="graph/index.php/signin/signout"><img border=no src="http://graph.coursepeer.com/images/cp_logout.png"></img></a> <?php } ?>
          </div>
        </div>
      </div>
    </div>

    <div class="container">

	<?php
		if($_SESSION['cp_usertype']=='Student' && $cp_loggedin==1){ // CoursePeer Usertype is 'Student'
	?>
      <div class="content">
        <div class="page-header">
          <h1>CoursePeer<small> Sample Application that uses Knowledge Graph API</small></h1>
        </div>
        <div class="row">
          <div class="span10">
            <h2>Hi, <?php echo $_SESSION['cp_name']; ?>!</h2><br />
			This is a demo.<br /><br />
			
			The following are your courses, as listed on your CoursePeer account: <br /><br />

			<?php
				$cp_request = new RestRequest('http://graph.coursepeer.com/index.php/data/users/me/courses?access_token='.$_SESSION['cp_access_token'],'POST');
				$cp_request->buildPostBody(array());
				$cp_request->execute();
				$result = $cp_request->getResponseBody();
				foreach($result as $courseid=>$coursedetails)
					echo "<a class=\"btn primary\" href=lectures.php?action=show&courseid=".$courseid."&coursename=".urlencode($coursedetails['course_code'].":".$coursedetails['course_name'])."><b>".$coursedetails['course_code'].":".$coursedetails['course_name']."</b></a><br /><br />";
			?>
			</div>
          <div class="span4">
			Your CoursePeer Profile data:<br />
			<h3><?php echo $_SESSION['cp_name']; ?></h3>
			<img border=no src="<?php echo $_SESSION['cp_profile_photo_url']; ?>" height=50 width=50></img><br />
			<h6><?php echo $_SESSION['cp_usertype']; ?></h6>
			<h5><?php echo $_SESSION['cp_major']; ?></h5>
			<h5><?php echo $_SESSION['cp_school'].", ".$_SESSION['cp_campus']; ?></h5>
			<h4><?php echo $_SESSION['cp_homecountry']; ?></h4><br />
          </div>
        </div>
      </div>
	<?php
		}
		elseif($_SESSION['cp_usertype']=='Instructor' && $cp_loggedin==1){ // CoursePeer Usertype is 'Instructor'
	?>
      <div class="content">
        <div class="page-header">
          <h1>CoursePeer<small> Sample Application that uses Knowledge Graph API</small></h1>
        </div>
        <div class="row">
          <div class="span10">
            <h2>Hi, <?php echo $_SESSION['cp_name']; ?>!</h2><br />
			This is a demo.<br /><br />
			
			The following are your courses, as listed on your CoursePeer account: <br /><br />

			<?php
				$cp_request = new RestRequest('http://graph.coursepeer.com/index.php/data/users/me/courses?access_token='.$_SESSION['cp_access_token'],'POST');
				$cp_request->buildPostBody(array());
				$cp_request->execute();
				$result = $cp_request->getResponseBody();
				foreach($result as $courseid=>$coursedetails)
					echo "<a class=\"btn primary\" href=lectures.php?action=show&courseid=".$courseid."&coursename=".urlencode($coursedetails['course_code'].":".$coursedetails['course_name'])."><b>".$coursedetails['course_code'].":".$coursedetails['course_name']."</b></a><br /><br />";
			?>
          </div>
          <div class="span4">
			Your CoursePeer Profile data:<br />
            <h3><?php echo $_SESSION['cp_name']; ?></h3>
			<img border=no src="<?php echo $_SESSION['cp_profile_photo_url']; ?>" height=50 width=50></img><br />
			<h6><?php echo $_SESSION['cp_usertype']; ?></h6>
			<h5><?php echo $_SESSION['cp_major']; ?></h5>
			<h5><?php echo $_SESSION['cp_school'].", ".$_SESSION['cp_campus']; ?></h5>
			<h4><?php echo $_SESSION['cp_homecountry']; ?></h4><br />
          </div>
        </div>
      </div>
	  <?php
			}
			else{?>
			  <div class="content">
				<div class="page-header">
				  <h1>CoursePeer<small> Sample Application that uses Knowledge Graph API</small></h1>
				</div>
				<div class="row">
				  <div class="span10">
				  <?php if(isset($_GET['error'])){ ?>
					<div class="alert-message error">
					  <a class="close" href="#">×</a>
					  <p><strong>Ooops!</strong> You need to authorize our CoursePeer application in order to be able to use the features of this site.</p>
					</div>
					<?php } ?>
					<h2>Welcome to CoursePeer Sample Application</h2><br />
					<br /><br />
					Our application is fully connected and integrated with CoursePeer, the social academic and professional career development platform.
					<br /><br /><br /><a href="graph/index.php/signin"><img border=no src="http://graph.coursepeer.com/images/cp_login.png"></img></a>
				  </div>
				</div>
			  </div>
			<?php
			}
		?>

      <footer>
        <p>&copy; Your Company 2011</p>
      </footer>

    </div> <!-- /container -->

  </body>
</html>