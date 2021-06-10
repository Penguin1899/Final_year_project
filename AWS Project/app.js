
// var is_looged_in = 0;

if ($("body").data("title") === "login") {

  var login_submit_button = document.querySelector('#login_btn')


  login_submit_button.onclick=function(){
  console.log('U have entered login_validation() function')
  login_validation();


  function login_validation(){
    var x = document.querySelector('#login_un').value
    var y = document.querySelector('#login_pwd').value
    var z = +new Date;
    var base_url = "https://7ncumltfm7.execute-api.ap-south-1.amazonaws.com/deploy"
    var get_url = base_url+"?username="+x+"&"+"password="+y+"&"+"timestamp="+z
    var validation_base_url="https://j7t32l24u7.execute-api.ap-south-1.amazonaws.com/deploy"
    var validation_url=validation_base_url + "?timestamp=" + z
    console.log('fetch URL is : ')
    console.log(get_url);
    console.log('the validation url is')
    console.log(validation_url)
    $.ajax({

          url: get_url,
          type: "GET",

          success: function json_func1(result)
          {

              console.log(result);
              validation();


              function validation(){
                $.ajax({
                  url: validation_url,
                  type: "GET",
                      //crossDomain:true,
                  success: function json_func2(result){

                  console.log(result);
                  //console.log(result['Timestamp'])
                  result_string = JSON.stringify(result);
                  //console.log(result_string);
                  if (result_string.includes("YES")){
                      console.log('User has been successfully validated');
                      window.location.replace("index.html")
                      //is_logged_in = 1;
                  }
                  else{

                      console.log('User has not been validated');
                      alert("Please enter the correct credentials");

                  }


              }



              })

          }}
      })

  }
  }
 }


 else{
  //For master control
  var masteroff_btn = document.querySelector('#masteroff_btn')
  var masteron_btn = document.querySelector('#masteron_btn')
  var masterreset_btn = document.querySelector('#masterreset_btn')


  var contentone =  document.querySelector('#Temp')
  var contentwo =  document.querySelector('#Humid')
  var par1,par2;

  var time_button = document.querySelector('#get_time')
  var time_data = document.querySelector("#on_time")

  var power_button = document.querySelector('#get_power')
  var power_data = document.querySelector("#power_consumed")

  var last_motion_button = document.querySelector('#last_motion_bt')
  var last_motion_data_time = document.querySelector('#last_motion_time')
  var last_motion_data_date = document.querySelector('#last_motion_date')

  var tb1_btn = document.querySelector('#get_tb1_data')
  var tb2_btn = document.querySelector('#get_tb2_data')
  var tb3_btn = document.querySelector('#get_tb3_data')

  var tb_1_arr = [0,0,0,0];
  var tb_3_arr = [0,0,0,0];
  var tb_2_arr = [0,0,0,0];


  //onclick event for master control

  masteroff_btn.onclick=function(){
  console.log('U have entered masteroff() function')
  masteroff();
  }

  masteron_btn.onclick=function(){
  console.log('U have entered masteron() function')
  masteron();
  }

  masterreset_btn.onclick=function(){
  console.log('U have entered masterreset() function')
  masterreset();
  }


  tb1_btn.onclick = function(){
    console.log('Getting table 1 data')
    api_call_tb1();
  }

  tb2_btn.onclick = function(){
    console.log('Getting table 2 data')
    api_call_tb2();
  }

  tb3_btn.onclick = function(){
    console.log('Getting table 3 data')
    api_call_tb3();
  }

  time_button.onclick = function(){
    console.log('getting time ON data...')
    api_call_time();
  }

  power_button.onclick = function(){
    console.log('getting power data...')
    api_call_power();
  }

  last_motion_button.onclick = function(){
    console.log('getting last motion data...')
    api_call_lastmotion();
  }


  setInterval(increment,600000);

  function increment()
  {
    api_call2();
  }

  $(document).ready(function api_call(){

        $.ajax({

              url: "https://x8jk6r2ro1.execute-api.ap-south-1.amazonaws.com/deploy",
              type: "GET",

              success: function json_func(result){

                  console.log(result);
                  console.log(result['0']);

                  par1 = result['0']['temp'];
                  par2 = result['0']['humidity'];

                  setTimeout(changeTextContent, 500)

              }
          })

  })



  // Functions for master control

  function masteroff(){
    var x = document.querySelector('#masteroff').value
    // console.log(x)
    var base_url = "https://1f9wlskr90.execute-api.ap-south-1.amazonaws.com/deploy?value=OFF&actuator_num="
    var final_url = base_url + x
    console.log("The url is",final_url)
    $.ajax({

      url: final_url,
      type: "GET",

      success: function json_func1(result)
      {
        console.log(result);
        var y = document.querySelector('#masteroff');
        y.value = ' ';
      },

      error: function err_func(xhr, status, error)
      {
        console.log('error occured');
        var y = document.querySelector('#masteroff');
        y.value = ' ';
      }
    })
  }

  function masteron(){
    var x = document.querySelector('#masteron').value
    // console.log(x)
    var base_url = "https://1f9wlskr90.execute-api.ap-south-1.amazonaws.com/deploy?value=ON&actuator_num="
    var final_url = base_url + x
    console.log("The url is",final_url)
    $.ajax({

      url: final_url,
      type: "GET",

      success: function json_func1(result)
      {
        console.log(result);
        var y = document.querySelector('#masteron');
        y.value = ' ';
      },

      error: function err_func(xhr, status, error)
      {
        console.log('error occured');
        var y = document.querySelector('#masteron');
        y.value = ' ';
      }
    })

  }

  function masterreset(){
    var x = document.querySelector('#masterreset').value
    // console.log(x)
    var base_url = "https://1f9wlskr90.execute-api.ap-south-1.amazonaws.com/deploy?value=RESET&actuator_num="
    var final_url = base_url + x
    console.log("The url is",final_url)
    $.ajax({

      url: final_url,
      type: "GET",

      success: function json_func1(result)
      {
        console.log(result);
        var y = document.querySelector('#masterreset');
        y.value = ' ';
      },

      error: function err_func(xhr, status, error)
      {
        console.log('error occured');
        var y = document.querySelector('#masterreset');
        y.value = ' ';
      }
    })
  }


  function api_call2(){

    $.ajax({

          url: "https://x8jk6r2ro1.execute-api.ap-south-1.amazonaws.com/deploy",
          type: "GET",

          success: function json_func2(result){

              console.log(result);
              console.log(result['0']);

              par1 = result['0']['temp'];
              par2 = result['0']['humidity'];

              setTimeout(changeTextContent, 500)

          }
      })

  }

  function changeTextContent()
  {
    contentone.textContent = "Temperature : "+par1+"°C";
    contentwo.textContent = "Humidity : "+par2+"%";
  }


  function api_call_time(){
    var x = document.querySelector('#dev_num1').value
    var y = document.querySelector('#act_num1').value
    var base_url = "https://wq351day1m.execute-api.ap-south-1.amazonaws.com/deploy"
    var get_url = base_url+"?light_num="+x+"&"+"act_num="+y
    console.log('fetch URL is : ')
    console.log(get_url);
    $.ajax({

          url: get_url,
          type: "GET",

          success: function json_func2(result){

              console.log(result);
              console.log(result['0']);

              par1 = result['0']['OnTime'];
              // par2 = result['0']['humidity'];

              setTimeout(changeTimeContext, 500)

          }
      })

  }

  function changeTimeContext(){
    time_data.textContent = par1+' mins';
  }

  function api_call_power(){
    var x = document.querySelector('#dev_num2').value
    var y = document.querySelector('#act_num2').value
    var base_url = "https://wq351day1m.execute-api.ap-south-1.amazonaws.com/deploy"
    var get_url = base_url+"?light_num="+x+"&"+"act_num="+y
    console.log('fetch URL is : ')
    console.log(get_url);
    $.ajax({

          url: get_url,
          type: "GET",

          success: function json_func2(result){

              console.log(result);
              console.log(result['0']);

              par1 = result['0']['OnTime'];
              par1 = (Number(par1)*3)/60;

              setTimeout(changePowerContext, 500)

          }
      })

  }

  function changePowerContext(){
    power_data.textContent = par1+' watts';
  }

  function api_call_lastmotion(){
    var x = document.querySelector('#dev_num3').value
    var y = document.querySelector('#act_num3').value
    var base_url = "https://odju6z2xg5.execute-api.ap-south-1.amazonaws.com/deploy"
    var get_url = base_url+"?dev_num="+x+"&"+"act_num="+y
    console.log('fetch URL is : ')
    console.log(get_url);
    $.ajax({

          url: get_url,
          type: "GET",

          success: function json_func2(result){

              console.log(result);
              console.log(result['0']);

              par1 = result['0']['timestamp'];
              par1 = Number(par1)+19800;
              let dateObj = new Date(par1*1000);
              par2 = dateObj.getDate()+'/'+(dateObj.getMonth()+1)+'/'+dateObj.getFullYear();
              let formattedDate = dateObj.getUTCHours()+':'+dateObj.getUTCMinutes()+':'+dateObj.getSeconds();
              par1 = formattedDate;
              // par1 = (Number(par1)*3)/60;

              setTimeout(changeMotionContext, 500)

          }
      })

  }

  function changeMotionContext(){
    last_motion_data_date.textContent = par2;
    last_motion_data_time.textContent = par1;
  }

  function sleep(ms) {
  return new Promise(resolve => setTimeout(resolve, ms));
}

  function api_call_tb1(){
    var ac_no = document.querySelector('#act_num_box1').value
    var base_url = "https://wq351day1m.execute-api.ap-south-1.amazonaws.com/deploy"

      var get_url = base_url+"?light_num="+1+"&"+"act_num="+ac_no
      console.log('fetch URL is : ')
      console.log(get_url);
      $.ajax({
            url: get_url,
            type: "GET",
            success: function json_func2(result){
                console.log(result);
                if(result['0']!=undefined){
                  console.log(result['0']);
                  // par1 = result['0']['OnTime'];;
                  tb_1_arr[0] = result['0']['OnTime'];
                }
                else {
                  tb_1_arr[0] = '-';
                }
                setTimeout(change_tb1_context, 500)
                // await sleep(1000);
            }
        })

        var get_url = base_url+"?light_num="+2+"&"+"act_num="+ac_no
        console.log('fetch URL is : ')
        console.log(get_url);
        $.ajax({
              url: get_url,
              type: "GET",
              success: function json_func2(result){
                  console.log(result);
                  if(result['0']!=undefined){
                    console.log(result['0']);
                    tb_1_arr[1] = result['0']['OnTime'];
                  }
                  else {
                    tb_1_arr[1] = '-';
                  }
                  setTimeout(change_tb1_context, 500)
              }
          })

          var get_url = base_url+"?light_num="+3+"&"+"act_num="+ac_no
          console.log('fetch URL is : ')
          console.log(get_url);
          $.ajax({
                url: get_url,
                type: "GET",
                success: function json_func2(result){
                    console.log(result);
                    if(result['0']!=undefined){
                      console.log(result['0']);
                      tb_1_arr[2] = result['0']['OnTime'];
                    }
                    else {
                      tb_1_arr[2] = '-';
                    }
                    setTimeout(change_tb1_context, 500)
                }
            })

          var get_url = base_url+"?light_num="+4+"&"+"act_num="+ac_no
          console.log('fetch URL is : ')
          console.log(get_url);
          $.ajax({
                url: get_url,
                type: "GET",
                success: function json_func2(result){
                    console.log(result);
                    if(result['0']!=undefined){
                      console.log(result['0']);
                      // par1 = result['0']['OnTime'];;
                      tb_1_arr[3] = result['0']['OnTime'];
                    }
                    else {
                      tb_1_arr[3] = '-';
                    }
                    setTimeout(change_tb1_context, 500)
                    // await sleep(1000);
                }
            })
  }

  function change_tb1_context() {
    console.log(tb_1_arr);

    if(tb_1_arr.length>4){tb_1_arr=[];}

    tb1_d1 = document.querySelector('#tb1_dev1');
    tb1_d2 = document.querySelector('#tb1_dev2');
    tb1_d3 = document.querySelector('#tb1_dev3');
    tb1_d4 = document.querySelector('#tb1_dev4');

    tb1_d1.textContent = tb_1_arr[0]+' mins';
    tb1_d2.textContent = tb_1_arr[1]+' mins';
    tb1_d3.textContent = tb_1_arr[2]+' mins';
    tb1_d4.textContent = tb_1_arr[3]+' mins';

  }

  function api_call_tb2(){
    var ac_no = document.querySelector('#act_num_box2').value
    var base_url = "https://wq351day1m.execute-api.ap-south-1.amazonaws.com/deploy"

      var get_url = base_url+"?light_num="+1+"&"+"act_num="+ac_no
      console.log('fetch URL is : ')
      console.log(get_url);
      $.ajax({
            url: get_url,
            type: "GET",
            success: function json_func2(result){
                console.log(result);
                if(result['0']!=undefined){
                  console.log(result['0']);
                  tb_2_arr[0] = result['0']['OnTime'];
                  tb_2_arr[0] = (Number(tb_2_arr[0])*3)/60;
                }
                else {
                  tb_2_arr[0] = '-';
                }
                setTimeout(change_tb2_context, 500)
            }
        })

        var get_url = base_url+"?light_num="+2+"&"+"act_num="+ac_no
        console.log('fetch URL is : ')
        console.log(get_url);
        $.ajax({
              url: get_url,
              type: "GET",
              success: function json_func2(result){
                  console.log(result);
                  if(result['0']!=undefined){
                    console.log(result['0']);
                    tb_2_arr[1] = result['0']['OnTime'];
                    tb_2_arr[1] = (Number(tb_2_arr[1])*3)/60;
                  }
                  else {
                    tb_2_arr[1] = '-';
                  }
                  setTimeout(change_tb2_context, 500)
              }
          })

          var get_url = base_url+"?light_num="+3+"&"+"act_num="+ac_no
          console.log('fetch URL is : ')
          console.log(get_url);
          $.ajax({
                url: get_url,
                type: "GET",
                success: function json_func2(result){
                    console.log(result);
                    if(result['0']!=undefined){
                      console.log(result['0']);
                      tb_2_arr[2] = result['0']['OnTime'];
                      tb_2_arr[2] = (Number(tb_2_arr[2])*3)/60;
                    }
                    else {
                      tb_2_arr[2] = '-';
                    }
                    setTimeout(change_tb2_context, 500)
                }
            })

          var get_url = base_url+"?light_num="+4+"&"+"act_num="+ac_no
          console.log('fetch URL is : ')
          console.log(get_url);
          $.ajax({
                url: get_url,
                type: "GET",
                success: function json_func2(result){
                    console.log(result);
                    if(result['0']!=undefined){
                      console.log(result['0']);
                      tb_2_arr[3] = result['0']['OnTime'];
                      tb_2_arr[3] = (Number(tb_2_arr[3])*3)/60;
                    }
                    else {
                      tb_2_arr[3] = '-';
                    }
                    setTimeout(change_tb2_context, 500)
                }
            })
  }

  function change_tb2_context(){
    console.log(tb_2_arr);

    if(tb_2_arr.length>4){tb_2_arr=[];}

    tb2_d1 = document.querySelector('#tb2_dev1');
    tb2_d2 = document.querySelector('#tb2_dev2');
    tb2_d3 = document.querySelector('#tb2_dev3');
    tb2_d4 = document.querySelector('#tb2_dev4');

    tb2_d1.textContent = tb_2_arr[0]+' watts';
    tb2_d2.textContent = tb_2_arr[1]+' watts';
    tb2_d3.textContent = tb_2_arr[2]+' watts';
    tb2_d4.textContent = tb_2_arr[3]+' watts';

  }

  function api_call_tb3(){
    var ac_no = document.querySelector('#act_num_box3').value
    var base_url = "https://odju6z2xg5.execute-api.ap-south-1.amazonaws.com/deploy"

      var get_url = base_url+"?dev_num="+1+"&"+"act_num="+ac_no
      console.log('fetch URL is : ')
      console.log(get_url);
      $.ajax({
            url: get_url,
            type: "GET",
            success: function json_func2(result){
                console.log(result);
                if(result['0']!=undefined){
                  console.log(result['0']);
                  tb_3_arr[0]=[0,0]
                  tb_3_arr[0][0] = result['0']['timestamp'];
                  tb_3_arr[0][0] = Number(tb_3_arr[0][0])+19800;
                  let dateObj = new Date(tb_3_arr[0][0]*1000);
                  tb_3_arr[0][1] = dateObj.getDate()+'/'+(dateObj.getMonth()+1)+'/'+dateObj.getFullYear();
                  let formattedDate = dateObj.getUTCHours()+':'+dateObj.getUTCMinutes()+':'+dateObj.getSeconds();
                  tb_3_arr[0][0] = formattedDate;
                }
                else {
                  tb_3_arr[0] = '-';
                }
                setTimeout(change_tb3_context, 500)
            }
        })

        var get_url = base_url+"?dev_num="+2+"&"+"act_num="+ac_no
        console.log('fetch URL is : ')
        console.log(get_url);
        $.ajax({
              url: get_url,
              type: "GET",
              success: function json_func2(result){
                  console.log(result);
                  if(result['0']!=undefined){
                    console.log(result['0']);
                    tb_3_arr[1]=[0,0]
                    tb_3_arr[1][0] = result['0']['timestamp'];
                    tb_3_arr[1][0] = Number(tb_3_arr[1][0])+19800;
                    let dateObj = new Date(tb_3_arr[1][0]*1000);
                    tb_3_arr[1][1] = dateObj.getDate()+'/'+(dateObj.getMonth()+1)+'/'+dateObj.getFullYear();
                    let formattedDate = dateObj.getUTCHours()+':'+dateObj.getUTCMinutes()+':'+dateObj.getSeconds();
                    tb_3_arr[1][0] = formattedDate;
                  }
                  else {
                    tb_3_arr[1] = '-';
                  }
                  setTimeout(change_tb3_context, 500)
              }
          })

          var get_url = base_url+"?dev_num="+3+"&"+"act_num="+ac_no
          console.log('fetch URL is : ')
          console.log(get_url);
          $.ajax({
                url: get_url,
                type: "GET",
                success: function json_func2(result){
                    console.log(result);
                    if(result['0']!=undefined){
                      console.log(result['0']);
                      tb_3_arr[2]=[0,0]
                      tb_3_arr[2][0] = result['0']['timestamp'];
                      tb_3_arr[2][0] = Number(tb_3_arr[2][0])+19800;
                      let dateObj = new Date(tb_3_arr[2][0]*1000);
                      tb_3_arr[2][1] = dateObj.getDate()+'/'+(dateObj.getMonth()+1)+'/'+dateObj.getFullYear();
                      let formattedDate = dateObj.getUTCHours()+':'+dateObj.getUTCMinutes()+':'+dateObj.getSeconds();
                      tb_3_arr[2][0] = formattedDate;
                    }
                    else {
                      tb_3_arr[2] = '-';
                    }
                    setTimeout(change_tb3_context, 500)
                }
            })

          var get_url = base_url+"?dev_num="+4+"&"+"act_num="+ac_no
          console.log('fetch URL is : ')
          console.log(get_url);
          $.ajax({
                url: get_url,
                type: "GET",
                success: function json_func2(result){
                    console.log(result);
                    if(result['0']!=undefined){
                      console.log(result['0']);
                      tb_3_arr[3]=[0,0]
                      tb_3_arr[3][0] = result['0']['timestamp'];
                      tb_3_arr[3][0] = Number(tb_3_arr[3][0])+19800;
                      let dateObj = new Date(tb_3_arr[3][0]*1000);
                      tb_3_arr[3][1] = dateObj.getDate()+'/'+(dateObj.getMonth()+1)+'/'+dateObj.getFullYear();
                      let formattedDate = dateObj.getUTCHours()+':'+dateObj.getUTCMinutes()+':'+dateObj.getSeconds();
                      tb_3_arr[3][0] = formattedDate;
                    }
                    else {
                      tb_3_arr[3] = '-';
                    }
                    setTimeout(change_tb3_context, 500)
                }
            })
  }

  function change_tb3_context(){
    console.log(tb_3_arr);

    if(tb_3_arr.length>4){tb_3_arr=[];}

    tb3_d1 = document.querySelector('#tb3_dev1');
    tb3_d2 = document.querySelector('#tb3_dev2');
    tb3_d3 = document.querySelector('#tb3_dev3');
    tb3_d4 = document.querySelector('#tb3_dev4');

    tb3_d1.textContent = tb_3_arr[0][1]+"\n"+tb_3_arr[0][0];
    tb3_d2.textContent = tb_3_arr[1][1]+"\n"+tb_3_arr[1][0];
    tb3_d3.textContent = tb_3_arr[2][1]+"\n"+tb_3_arr[2][0];
    tb3_d4.textContent = tb_3_arr[3][1]+"\n"+tb_3_arr[3][0];

  }

 }
