/*
 * FileName:ColorCardActivity
 * 引用声明：参考文献：Android应用开发入门   参考资料：网络资源（在此对c论坛的***表示感谢！！！）
 * 文件声明：这份文件是基于安卓色卡应用程序的修改升级
 * 作者：匿名
 * 联系方式：QQ：125 521 0185
 * 时间：2017.9.27
 * 版本兼容声明：Android4.4及上下  建议Android4.4以上运行
 * 编译：API 18 sdk：15-21
 * 语言兼容：仅支持简易中文
 * 
 * */

package com.example.colorcard;

import java.io.File;
import java.io.IOException;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.UUID;

import android.app.Activity;
import android.app.AlertDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.ContentResolver;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.graphics.drawable.BitmapDrawable;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.TableLayout;
import android.widget.TableRow;
import android.widget.TextView;
import android.widget.Toast;

public class ColorCardActivity extends Activity {
	//三个选项卡的定义
	private LinearLayout sampleTab = null;
	private LinearLayout searchTab = null;
	private LinearLayout identifyTab = null;	
	private LinearLayout[] tabs;

	//选项卡对应界面
	private View sampleTabView = null;
	private View searchTabView = null;
	private View identifyTabView = null;
	//选项卡下方布局
	private LinearLayout content = null;
	//色卡数据表定义
	private List<ColorSample>sampleList = new ArrayList<ColorSample>();
	//照片选取功能定义
	public static final int PHOTO_CAPTURE = 100;//拍照
	public static final int PHOTO_CROP = 200;//剪裁
	public static final int PHOTO_REQUEST_GALLERY = 300;// 从相册中选择
	
	BluetoothAdapter bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
	BluetoothDevice bluetoothDevice;
	BluetoothSocket bluetoothSocket = null;
	OutputStream outputStream = null;
	private BluetoothAdapter ba;
	private static final UUID MY_UUID_SECURE=UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    private String blueAddress="98:D3:31:FD:C3:31";//蓝牙模块的MAC地址
	
	private View imageview;//显示图片控件
	private Bitmap pickbmp;//图片拾色
//	uri——资源引用方法加载图片文件缓存
    private File capfile;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_color_card);
		//初始化选项卡界面控件_通过控件ID获得控件对象
		sampleTab = (LinearLayout)findViewById(R.id.sampleTab);
		searchTab = (LinearLayout)findViewById(R.id.searchTab);
		identifyTab = (LinearLayout)findViewById(R.id.identifyTab);
		tabs = new LinearLayout[]{sampleTab,searchTab,identifyTab};
	
		//初始化选项卡对应的界面布局
		LayoutInflater factory = LayoutInflater.from(this);
		sampleTabView = factory.inflate(R.layout.color_sample,null);
		searchTabView = factory.inflate(R.layout.color_search,null);
		identifyTabView = factory.inflate(R.layout.color_identify,null);
//		//默认显示界面
		content = (LinearLayout)findViewById(R.id.content);
		content.addView(sampleTabView);

		//界面及组件功能初始化
		loadColorCards();
		initSampleTabView();
		initSearchTabView();
		initIdentifyTableView();
		
		setTabChecked(identifyTab);
		
		//清楚原始界面，加载选中界面
		content.removeAllViews();
		content.addView(identifyTabView);
		//加载自定义动画
		Animation animation =AnimationUtils.loadAnimation
				(ColorCardActivity.this, R.anim.myanim);
		//在组件上运用
		identifyTabView.startAnimation(animation);	
		
		ba = BluetoothAdapter.getDefaultAdapter();
		
		if (ba == null) {
			Toast.makeText(ColorCardActivity.this, "该设备不支持蓝牙！", Toast.LENGTH_SHORT).show();
        }
 
        // When Bluetooth hasn't opened, enable it
        if (!ba.isEnabled()) {
        	Toast.makeText(ColorCardActivity.this, "蓝牙未开启！", Toast.LENGTH_SHORT).show();
        	Intent turnOn = new Intent(
            BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(turnOn, 0);
        }
		
		
		
	
		//单击选择色卡界面——//	匿名内部类单击事件响应
		sampleTab.setOnClickListener(new OnClickListener(){
			@Override
			public void onClick(View v) {
				//选项卡重置
				setTabChecked(sampleTab);
				
				//清楚原始界面，加载选中界面
				content.removeAllViews();
				content.addView(sampleTabView);
				//加载自定义动画
				Animation animation =AnimationUtils.loadAnimation
						(ColorCardActivity.this, R.anim.myanim);
				//在组件上运用
				sampleTabView.startAnimation(animation);
			}	
		});
		
		
		//单击选择搜索界面
		searchTab.setOnClickListener(new OnClickListener(){

			@Override
			public void onClick(View v) {
				//选项卡重置
				//选项卡重置
				setTabChecked(searchTab);
				
				//清楚原始界面，加载选中界面
				content.removeAllViews();
				content.addView(searchTabView);
				//加载自定义动画
				Animation animation =AnimationUtils.loadAnimation
						(ColorCardActivity.this, R.anim.myanim);
				//在组件上运用
				searchTabView.startAnimation(animation);	
			}
		});
		
		//单击选择辨色界面
		identifyTab.setOnClickListener(new OnClickListener(){

					@Override
					public void onClick(View v) {
						//选项卡重置
						//选项卡重置
						setTabChecked(identifyTab);
						
						//清楚原始界面，加载选中界面
						content.removeAllViews();
						content.addView(identifyTabView);
						//加载自定义动画
						Animation animation =AnimationUtils.loadAnimation
								(ColorCardActivity.this, R.anim.myanim);
						//在组件上运用
						identifyTabView.startAnimation(animation);	
					}
				});
		
		
	}//onCreate到此结束
	
	private void initIdentifyTableView() {
		// TODO 自动生成的方法存根
		//各组件定义及初始化
		final TextView sample01 = (TextView)identifyTabView.findViewById(R.id.sample01);
		final TextView sample02 = (TextView)identifyTabView.findViewById(R.id.sample02);
		final TextView sample03 = (TextView)identifyTabView.findViewById(R.id.sample03);
		
		final TextView textColorDiff = (TextView)identifyTabView.findViewById(R.id.textColorDiff);
		final TextView textColorInfo = (TextView)identifyTabView.findViewById(R.id.textColorinfo);
		
		final EditText editTextRGB = (EditText)identifyTabView.findViewById(R.id.editTextRGB);
		
		final View viewPickedColor = (View)identifyTabView.findViewById(R.id.viewPickedColor);
	    imageview = (View)identifyTabView.findViewById(R.id.viewPicture);
	    
	    Button btnSetmode = (Button)identifyTabView.findViewById(R.id.btnSTEMODE);
	    Button btnCamera = (Button)identifyTabView.findViewById(R.id.btnCamera);
	    Button btnRest = (Button)identifyTabView.findViewById(R.id.btnRest);
	    Button photo_album = (Button)identifyTabView.findViewById(R.id.photo_album);
	    Button btnSetdata = (Button)identifyTabView.findViewById(R.id.btnSETDATA);
	    final Button Link_Set = (Button)identifyTabView.findViewById(R.id.bt_link);
//	   	 修改uri此处获取存储，并命名图片文件temp————注意储存权限读取声明
	    capfile = new File(Environment.getExternalStorageDirectory(),"temp.jpg");

	    //初始化图片默认_加载背景图片
	    Bitmap bmp = BitmapFactory.decodeResource(getResources(), R.drawable.bg01);
	    imageview = identifyTabView.findViewById(R.id.viewPicture);
	    imageview.setBackgroundDrawable(new BitmapDrawable(bmp));
		//从触摸开始辨色数据_格式#XXXXXX_RGB（X为辨色数据）_捕捉触点
	    imageview.setOnTouchListener(new View.OnTouchListener() {	
			@Override
			public boolean onTouch(View v, MotionEvent event) {
				// TODO 自动生成的方法存根
				//定义坐标
				int x=(int)event.getX();
				int y=(int)event.getY();
				int h=imageview.getHeight();
				int w=imageview.getWidth();
				//判断触摸范围
				if(x<0||x>=w||y<0||y>=h){
					textColorInfo.setText("超出范围!");
					return true;
				}
				//若未初始化图像，则先获取图像数据
					if(pickbmp ==null){
						Bitmap bgbmp=((BitmapDrawable)imageview.getBackground()).getBitmap();
						pickbmp = Bitmap.createScaledBitmap(bgbmp,w,h,false);
					}
					//显示当前像素颜色
					int pixel=pickbmp.getPixel(x, y);
						
				    viewPickedColor.setBackgroundColor(pixel);
				    //显示16进制RGB值，格式#XXXXXX_RGB（X为辨色数据）
				    String rgb ="#"+Integer.toHexString(pixel).substring(2).toUpperCase();
					
				    textColorInfo.setText("辨色数据："+rgb+"(格式#_R_G_B)" );
				    editTextRGB.setText(rgb);
				    Bluetooth_transmission();
					int r=Color.red(pixel);
				    int g=Color.green(pixel);
				    int b=Color.blue(pixel);
				    float[] hsv=new float[3];
				    Color.RGBToHSV(r, g, b, hsv);
				    //
				    //TODO获取最近三种色卡并显示在界面上
				    //
				    // 将当前触摸颜色与各色卡的“距离”即色差值存放到Map中n
					Map<Double, ColorSample> mapHSV = new HashMap<Double, ColorSample>();
					//遍历整个色卡数据表查找相似色
					for (ColorSample sample : sampleList) {
						double dHSV = ColorSample.distHSV(hsv[0], hsv[1], hsv[2],
								sample.h, sample.s, sample.v);
						// 增加一个随机的微量值，避免重复，因为可能存在多个色卡与指定颜色的距离相等
						dHSV = dHSV + Math.random() / 1000000.0;
						mapHSV.put(dHSV, sample);
					}
					// 对颜色的色差值进行大小排序
					List<Double> distHSVList = new ArrayList<Double>(mapHSV
							.keySet());
					Collections.sort(distHSVList);
					// 得到最接近的三种颜色
					ColorSample hitted01 = mapHSV.get(distHSVList.get(0));
					ColorSample hitted02 = mapHSV.get(distHSVList.get(1));
					ColorSample hitted03 = mapHSV.get(distHSVList.get(2));
					//保留四位小数格式打印
					String diff01 = String.format("%.4f", distHSVList.get(0));
					String diff02 = String.format("%.4f", distHSVList.get(1));
					String diff03 = String.format("%.4f", distHSVList.get(2));
					textColorDiff.setText("差异值：" + diff01 + ", " + diff02 + ", "
							+ diff03);
					// 显示找到的三种颜色
					sample01.setBackgroundColor(hitted01.val);
					sample02.setBackgroundColor(hitted02.val);
					sample03.setBackgroundColor(hitted03.val);
					sample01.setText(hitted01.rgb + "\n" + hitted01.name);
					sample02.setText(hitted02.rgb + "\n" + hitted02.name);
					sample03.setText(hitted03.rgb + "\n" + hitted03.name);
					// 清空内存
					distHSVList.clear();
					mapHSV.clear();
				    
				return true;
			}
		});
	    
	  //单击键“模式” 触发事件
	    btnSetmode.setOnClickListener(new OnClickListener(){
			@Override
			public void onClick(View v) {
				// TODO 自动生成的方法存根
				//发送修改模式
				if(bluetoothSocket!=null) {
				String send_str = " SET_MODE\r\n";
				byte[] bf = new byte[32];
				bf = send_str.getBytes();//转换为字节
				 try {
              	   outputStream = bluetoothSocket.getOutputStream();
              	   outputStream.write(bf);
              	   outputStream.write('\0');    // Send an ending sign
                 } catch (IOException e) {
                     e.printStackTrace();
                     Toast.makeText(ColorCardActivity.this, "蓝牙配置失败!", Toast.LENGTH_SHORT).show();
                     return;
                 }
                 //强行输出，清空缓存区
                 try {
                     if (outputStream != null) {
                  	   outputStream.flush();
                     }
                 } catch (IOException e) {
                     e.printStackTrace();
                     //缓存清理失败
                     Toast.makeText(ColorCardActivity.this, "缓存清理失败", Toast.LENGTH_SHORT).show();
                 }
             }else{
          	   Toast.makeText(ColorCardActivity.this, "请先建立蓝牙连接", Toast.LENGTH_SHORT).show();
          	   return;
             }
			}
	    	
	    });
	    //
	    //
	    //TODO申请系统权限——以启用相机拍照
	    //
	    //单击键“拍照” 触发事件
	    btnCamera.setOnClickListener(new OnClickListener(){
			@Override
			public void onClick(View v) {
				// TODO 自动生成的方法存根
				//启用系统相机
				Intent intent  = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
//				//指明返回数据，这里就是照片
//				intent.putExtra("return-data", true);
//				修改uri方法调用
				intent.putExtra(MediaStore.EXTRA_OUTPUT,Uri.fromFile(capfile));
				startActivityForResult(intent,PHOTO_CAPTURE);

			}
	    	
	    });
	    
	  //单击键“重置” 触发事件
	    btnRest.setOnClickListener(new OnClickListener(){
			@Override
			public void onClick(View v) {
				// TODO 自动生成的方法存根
				//初始化图片默认_加载背景图片
			    Bitmap bmp = BitmapFactory.decodeResource(getResources(), R.drawable.bg01);
			    imageview = identifyTabView.findViewById(R.id.viewPicture);
			    imageview.setBackgroundDrawable(new BitmapDrawable(bmp));
			    pickbmp = null;

			}
	    	
	    });
	    //
	    //TODO申请系统权限——以启用相册选取图片
	    //
	    //单击键“相册” 触发事件
	    photo_album.setOnClickListener(new OnClickListener(){
			@Override
			public void onClick(View v) {
				// TODO 自动生成的方法存根
//				此方法兼容4.4上下版本——引用说明——来自c论坛***网络资源——在此表示感谢!!!
				Intent intent = new Intent(Intent.ACTION_PICK,android.provider.MediaStore.Images.Media.EXTERNAL_CONTENT_URI);
				intent.setDataAndType(MediaStore.Images.Media.EXTERNAL_CONTENT_URI,"image/*");
				startActivityForResult(intent,PHOTO_REQUEST_GALLERY);
				//启用系统相册
//                Intent intent = new Intent(Intent.ACTION_GET_CONTENT); //4.4推荐用此方式，4.4以下的API需要再兼容
//                intent.addCategory(Intent.CATEGORY_OPENABLE);
//                intent.setType("image/*");
			}	    	
	    });
	    
	    
	  //单击键“设置” 触发事件
	    btnSetdata.setOnClickListener(new OnClickListener(){
			@Override
			public void onClick(View v) {
				// TODO 自动生成的方法存根
				//发送RGB数据
				Bluetooth_transmission();
				
			}
	    	
	    });
	    //
	    
	  //响应连接建立
	    Link_Set.setOnClickListener(new OnClickListener(){
	  					@Override
	  					public void onClick(View v) {
	  						// TODO 自动生成的方法存根
	  						//建立蓝牙数据服务
	  						Set<BluetoothDevice> devices = bluetoothAdapter.getBondedDevices();
	  				        bluetoothDevice = bluetoothAdapter.getRemoteDevice(blueAddress);
	  				        if(bluetoothSocket==null){
	  				        	try{
	  						           bluetoothSocket = bluetoothDevice.createRfcommSocketToServiceRecord(MY_UUID_SECURE);
	  						           bluetoothSocket.connect();
	  						         Link_Set.setText("OK");
	  						       }catch (IOException e){
	  						           e.printStackTrace();
	  						         Link_Set.setText("连接");
	  						           Toast.makeText(ColorCardActivity.this, "蓝牙连接建立失败!", Toast.LENGTH_SHORT).show();
	  						       }
	  				        }
	  				        else {
	  				        	//Toast.makeText(ig_m.this, "已连接蓝牙配置服务", Toast.LENGTH_SHORT).show();
	  				        }	        	

	  					}	
	  				});
	    
	    
	}
	//系统 覆盖、/实现方法
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		// TODO 自动生成的方法存根
		//如果中途取消对照片的处理，则直接退出
		if(resultCode==Activity.RESULT_CANCELED){
			return;
		}
		//若从拍照处返回，则启用系统剪裁
		else if(requestCode==PHOTO_CAPTURE){	
//				修改方法uri调用
				Intent intent = new Intent("com.android.camera.action.CROP");
//				设置数据及类型_这里是指uri返回的图片
				intent.setDataAndType(Uri.fromFile(capfile), "image/*");
				//适应imageview剪裁
				intent.putExtra("aspectX",imageview.getWidth());
				intent.putExtra("aspectY",imageview.getHeight());
				//输出剪裁后图片大小_这里是适应imageview
				intent.putExtra("outputX",imageview.getWidth());
				intent.putExtra("outputY",imageview.getHeight());
				//2.使用Uri资源引用图片数据
				intent.putExtra("output", Uri.fromFile(capfile));
				intent.putExtra("outputFormat","JPEG");
				// 开启一个带有返回值的Activity，请求码为PHOTO_CROP
				startActivityForResult(intent,PHOTO_CROP);

		}
		
//		剪裁返回
		else if (requestCode == PHOTO_CROP){
//			修改uri方法
			if(capfile.exists()){
				Bitmap photo = BitmapFactory.decodeFile(capfile.getAbsolutePath());
				imageview.setBackgroundDrawable(new BitmapDrawable(photo));
			    pickbmp = null;
			}
		}
//		相册返回
		else if (requestCode == PHOTO_REQUEST_GALLERY){
			 // 从相册返回的数据
			 if (data != null&&resultCode == Activity.RESULT_OK) {
//				 String[] projection = { MediaStore.Images.Media.DATA };
//				    Cursor cursor = context.getContentResolver().query(contentUri, projection, null, null, null);
//				    int column_index = cursor.getColumnIndexOrThrow(MediaStore.Images.Media.DATA);
//				    cursor.moveToFirst();
//				    filePath = cursor.getString(column_index);
				 //得到缓存数据
				 Uri uri = data.getData();
//				 String[] pojo = {MediaStore.Images.Media.DATA};
//				 Cursor cursor = managedQuery(uri, pojo, null, null, null);
//				 if (cursor != null) {
//					 ContentResolver cr = this.getContentResolver();
//                     int colunm_index = cursor.getColumnIndexOrThrow(MediaStore.Images.Media.DATA);
//                     cursor.moveToFirst();
//                     String path = cursor.getString(colunm_index);
//			 }
//                     //以上代码获取图片路径
                     
     				Intent intent = new Intent("com.android.camera.action.CROP");
//    				设置数据及类型_这里是指uri返回的图片
    				intent.setDataAndType(uri, "image/*");
    				//适应imageview剪裁
    				intent.putExtra("aspectX",imageview.getWidth());
    				intent.putExtra("aspectY",imageview.getHeight());
    				//输出剪裁后图片大小_这里是适应imageview
    				intent.putExtra("outputX",imageview.getWidth());
    				intent.putExtra("outputY",imageview.getHeight());
    				//2.使用Uri输出图片数据
    				intent.putExtra("output", Uri.fromFile(capfile));
    				intent.putExtra("outputFormat","JPEG");
    				// 开启一个带有返回值的Activity，请求码为PHOTO_CROP
    				startActivityForResult(intent,PHOTO_CROP);
                    
//				 Toast.makeText(ColorCardActivity.this,"版本兼容易出错，此段程序修改_andriod4.4上下兼容",
//							Toast.LENGTH_SHORT).show();
//				拍照内存溢出（OOM）完美解决_nice
//    				添加功能_相册选取照片
				 
			 }
				
		}
		
		super.onActivityResult(requestCode, resultCode, data);
	}
	
	/**重构函数
	 * 发送RGB数据
	 * 通过蓝牙发送数据到下位机设备
	 */
	private void Bluetooth_transmission(){
		//发送
		final EditText editTextRGB = (EditText)identifyTabView.findViewById(R.id.editTextRGB);
		if(bluetoothSocket!=null) {
		String send_str = " ";
		send_str = send_str+editTextRGB.getText()+" \r\n";
		byte[] bf = new byte[64];
		bf = send_str.getBytes();//转换为字节
		 try {
      	   outputStream = bluetoothSocket.getOutputStream();
      	   outputStream.write(bf);
      	   outputStream.write('\0');    // Send an ending sign
         } catch (IOException e) {
             e.printStackTrace();
             Toast.makeText(ColorCardActivity.this, "蓝牙配置失败!", Toast.LENGTH_SHORT).show();
             return;
         }
         //强行输出，清空缓存区
         try {
             if (outputStream != null) {
          	   outputStream.flush();
             }
         } catch (IOException e) {
             e.printStackTrace();
             //缓存清理失败
             Toast.makeText(ColorCardActivity.this, "缓存清理失败", Toast.LENGTH_SHORT).show();
         }
     }else{
  	   Toast.makeText(ColorCardActivity.this, "请先建立蓝牙连接", Toast.LENGTH_SHORT).show();
  	   return;
     }
	}

	/**重构函数
	 * 设置Tab被单击时的属性状态
	 * 显示被单击时的选项卡
	 *
	 */
	public void setTabChecked(LinearLayout tab){
		for (int i=0;i<tabs.length;i++){
			tabs[i].setBackgroundDrawable(null);
			TextView txt = (TextView) tabs[i].getChildAt(0);
			txt.setTextColor(getResources().getColor(R.color.darkgreen));
		}
		tab.setBackgroundResource(R.drawable.tabselected);
		TextView txt = (TextView) tab.getChildAt(0);
		txt.setTextColor(getResources().getColor(R.color.white));
	}
	
	/**重构函数
	 * 获取色卡数据
	 */
	private void loadColorCards(){
		//获取数据按“\n”进行分割
		String sampleColors = getResources().getString(
				R.string.sample_color_list);
		String[] ss = sampleColors.split("\n");
		//解释并转换色卡数据  格式【十六进制RGB】【颜色名】[所属类别]
		String rgb,name,category;
		int i,j;
		for(String s:ss){
			rgb = name = category =  null;
			if(s.trim().length()>0){
				i = s.indexOf('[');
				j = s.indexOf(']');
				if(j>i&&i>=0){
					rgb = s.substring(i+1,j);
				}
				i = s.indexOf('[',j);
				j = s.indexOf(']',i);
				if(j>i&&i>=0){
					name = s.substring(i+1, j);
				}
				i = s.indexOf('[',j);
				j = s.indexOf(']',i);
				if(j>i&&i>=0){
					category = s.substring(i+1, j);
				}
				//保存有效色卡
				if(rgb!=null&&name!=null&&category!=null){
					sampleList.add(new ColorSample(rgb,name,category));
				}
			}
		}	
	}
	/**重构函数
	 * 初始化色卡界面
	 * 保存色卡数据来源到sampleList
	 */
	private void initSampleTabView(){
		//获取像素密度
		final float scale = getResources().getDisplayMetrics().density;
		//清空源色卡
		TableLayout sampleTable = (TableLayout)sampleTabView.findViewById(R.id.sampleTable);
		sampleTable.removeAllViews();
		//动态添加色卡
		for(final ColorSample samp:sampleList){
			TableRow row = new TableRow(this);
			row.setPadding(0, (int)(20*scale+0.5f), 0, 0);
			View co100 = new View(this);
			co100.setBackgroundColor(samp.val);
			co100.setMinimumHeight((int)(80*scale+0.5f));
			TextView co101 = new TextView(this);
			co101.setText(samp.name);
			//字体大小颜色调节
			co101.setTextSize(20);
			co101.setTextColor(Color.DKGRAY);
			co101.setGravity(Gravity.CENTER);
			co101.setHeight((int)(80*scale+0.5f));
			row.addView(co100);
			row.addView(co101);
			sampleTable.addView(row);  
		}
	}
	
	/**构架函数
	 * 搜索函数
	 */
	private void initSearchTabView(){
		//获取输入的内容，显示等   注意所有使用到的组件必须定义来源_否则程序将强行停止运行
		Button btnSearch =(Button)searchTabView.findViewById(R.id.btnSearch);
		final TableLayout resultTable = 
				(TableLayout)searchTabView.findViewById(R.id.resultTable);
		final EditText editColorName = 
				(EditText)searchTabView.findViewById(R.id.editColorName);
		//单击"搜索"按键触发事件——进行搜索
		btnSearch.setOnClickListener(new OnClickListener(){
			@Override
			public void onClick(View v) {
				//创建色卡数据表并清空——显示用
				List<ColorSample> sampleList_result=new ArrayList<ColorSample>();	
				
				//尝试搜索并把符合条件的显示出来_异常则提示出错
				try{
					//获取字符如果有"色"字去掉“色”字，以实现模糊搜索，并避免把所有项都符合条件
					String search_con=editColorName.getText().toString();
	            	if (search_con.contains("色")){
	            		//字符串取子串去掉“色”字
	            		search_con = search_con.substring(0, search_con.length()-1);
	            	}
	            	//遍历整个色卡数据表，如果”字“匹配成功，则添加到sampleList_result清单中
	            	for(final ColorSample samp2: sampleList){
	            		if(samp2.name.contains(search_con)==true){
	            			sampleList_result.add(samp2);
	            		}
	            	}
	            	//获取像素密度
	                final float scale=getResources().getDisplayMetrics().density;
	        	    resultTable.removeAllViews();
	            	//创建动态色卡，用于加载色卡搜索结果数据-详情参考sample界面初始化程序与书面上解释
	            	for(final ColorSample samp3:sampleList_result){
	        			TableRow row = new TableRow(ColorCardActivity.this);
	        			row.setPadding(0, (int)(20*scale+0.5f), 0, 0);
	        			View co100 = new View(ColorCardActivity.this);
	        			co100.setBackgroundColor(samp3.val);
	        			co100.setMinimumHeight((int)(80*scale+0.5f));
	        			TextView co101 = new TextView(ColorCardActivity.this);
	        			co101.setText(samp3.name);
	        			//字体大小颜色调节
	        			co101.setTextSize(20);
	        			co101.setTextColor(Color.DKGRAY);
	        			
	        			co101.setGravity(Gravity.CENTER);
	        			co101.setHeight((int)(80*scale+0.5f));
	        			row.addView(co100);
	        			row.addView(co101);
	        			resultTable.addView(row);  
	        			
	        		}
				}
				//异常处理
				catch (Exception e){
					Toast.makeText
					(ColorCardActivity.this,"搜索未知错误,请检查!",Toast.LENGTH_SHORT).show();
				}
			}
		});
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.color_card, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// Handle action bar item clicks here. The action bar will
		// automatically handle clicks on the Home/Up button, so long
		// as you specify a parent activity in AndroidManifest.xml.
		int id = item.getItemId();
		if (id == R.id.action_settings) {
			//添加功能退出
		       try {
		              // 将临时图片文件删除
			          capfile.delete();
		       }
			       catch (Exception e) {
			    	   //若失败则打印错误信息在程序中出错的位置及原因
			             e.printStackTrace();
				         }	
			finish();
			return true;
		}
		return super.onOptionsItemSelected(item);
	}

	@Override
//	自动生成方法——退出确定
	public void onBackPressed() {
		// TODO 自动生成的方法存根
        new AlertDialog.Builder(this).setTitle("图片文件将被删除，确认退出吗？") 
        .setIcon(android.R.drawable.ic_dialog_info) 
        .setPositiveButton("确定", new DialogInterface.OnClickListener() { 
 
            @Override 
            public void onClick(DialogInterface dialog, int which) { 
            // 点击“确认”后的操作 
 		       try {
		              // 将临时图片文件删除
			          capfile.delete();
		       }
			       catch (Exception e) {
			    	   //若失败则打印错误信息在程序中出错的位置及原因
			             e.printStackTrace();
				         }	
             finish(); 
     
            } 
        }) 
        .setNegativeButton("再看看", new DialogInterface.OnClickListener() { 
     
            @Override 
            public void onClick(DialogInterface dialog, int which) { 
            // 点击“返回”后的操作,这里不设置没有任何操作 
            } 
        }).show(); 
        
//		super.onBackPressed();
	}
}
