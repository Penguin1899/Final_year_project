
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
  
  
  setInterval(increment,10000);
  
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
  


 }





