/*
 * FileName:ColorCardActivity
 * �����������ο����ף�AndroidӦ�ÿ�������   �ο����ϣ�������Դ���ڴ˶�c��̳��***��ʾ��л��������
 * �ļ�����������ļ��ǻ��ڰ�׿ɫ��Ӧ�ó�����޸�����
 * ���ߣ�����
 * ��ϵ��ʽ��QQ��125 521 0185
 * ʱ�䣺2017.9.27
 * �汾����������Android4.4������  ����Android4.4��������
 * ���룺API 18 sdk��15-21
 * ���Լ��ݣ���֧�ּ�������
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
	//����ѡ��Ķ���
	private LinearLayout sampleTab = null;
	private LinearLayout searchTab = null;
	private LinearLayout identifyTab = null;	
	private LinearLayout[] tabs;

	//ѡ���Ӧ����
	private View sampleTabView = null;
	private View searchTabView = null;
	private View identifyTabView = null;
	//ѡ��·�����
	private LinearLayout content = null;
	//ɫ�����ݱ���
	private List<ColorSample>sampleList = new ArrayList<ColorSample>();
	//��Ƭѡȡ���ܶ���
	public static final int PHOTO_CAPTURE = 100;//����
	public static final int PHOTO_CROP = 200;//����
	public static final int PHOTO_REQUEST_GALLERY = 300;// �������ѡ��
	
	BluetoothAdapter bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
	BluetoothDevice bluetoothDevice;
	BluetoothSocket bluetoothSocket = null;
	OutputStream outputStream = null;
	private BluetoothAdapter ba;
	private static final UUID MY_UUID_SECURE=UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    private String blueAddress="98:D3:31:FD:C3:31";//����ģ���MAC��ַ
	
	private View imageview;//��ʾͼƬ�ؼ�
	private Bitmap pickbmp;//ͼƬʰɫ
//	uri������Դ���÷�������ͼƬ�ļ�����
    private File capfile;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_color_card);
		//��ʼ��ѡ�����ؼ�_ͨ���ؼ�ID��ÿؼ�����
		sampleTab = (LinearLayout)findViewById(R.id.sampleTab);
		searchTab = (LinearLayout)findViewById(R.id.searchTab);
		identifyTab = (LinearLayout)findViewById(R.id.identifyTab);
		tabs = new LinearLayout[]{sampleTab,searchTab,identifyTab};
	
		//��ʼ��ѡ���Ӧ�Ľ��沼��
		LayoutInflater factory = LayoutInflater.from(this);
		sampleTabView = factory.inflate(R.layout.color_sample,null);
		searchTabView = factory.inflate(R.layout.color_search,null);
		identifyTabView = factory.inflate(R.layout.color_identify,null);
//		//Ĭ����ʾ����
		content = (LinearLayout)findViewById(R.id.content);
		content.addView(sampleTabView);

		//���漰������ܳ�ʼ��
		loadColorCards();
		initSampleTabView();
		initSearchTabView();
		initIdentifyTableView();
		
		setTabChecked(identifyTab);
		
		//���ԭʼ���棬����ѡ�н���
		content.removeAllViews();
		content.addView(identifyTabView);
		//�����Զ��嶯��
		Animation animation =AnimationUtils.loadAnimation
				(ColorCardActivity.this, R.anim.myanim);
		//�����������
		identifyTabView.startAnimation(animation);	
		
		ba = BluetoothAdapter.getDefaultAdapter();
		
		if (ba == null) {
			Toast.makeText(ColorCardActivity.this, "���豸��֧��������", Toast.LENGTH_SHORT).show();
        }
 
        // When Bluetooth hasn't opened, enable it
        if (!ba.isEnabled()) {
        	Toast.makeText(ColorCardActivity.this, "����δ������", Toast.LENGTH_SHORT).show();
        	Intent turnOn = new Intent(
            BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(turnOn, 0);
        }
		
		
		
	
		//����ѡ��ɫ�����桪��//	�����ڲ��൥���¼���Ӧ
		sampleTab.setOnClickListener(new OnClickListener(){
			@Override
			public void onClick(View v) {
				//ѡ�����
				setTabChecked(sampleTab);
				
				//���ԭʼ���棬����ѡ�н���
				content.removeAllViews();
				content.addView(sampleTabView);
				//�����Զ��嶯��
				Animation animation =AnimationUtils.loadAnimation
						(ColorCardActivity.this, R.anim.myanim);
				//�����������
				sampleTabView.startAnimation(animation);
			}	
		});
		
		
		//����ѡ����������
		searchTab.setOnClickListener(new OnClickListener(){

			@Override
			public void onClick(View v) {
				//ѡ�����
				//ѡ�����
				setTabChecked(searchTab);
				
				//���ԭʼ���棬����ѡ�н���
				content.removeAllViews();
				content.addView(searchTabView);
				//�����Զ��嶯��
				Animation animation =AnimationUtils.loadAnimation
						(ColorCardActivity.this, R.anim.myanim);
				//�����������
				searchTabView.startAnimation(animation);	
			}
		});
		
		//����ѡ���ɫ����
		identifyTab.setOnClickListener(new OnClickListener(){

					@Override
					public void onClick(View v) {
						//ѡ�����
						//ѡ�����
						setTabChecked(identifyTab);
						
						//���ԭʼ���棬����ѡ�н���
						content.removeAllViews();
						content.addView(identifyTabView);
						//�����Զ��嶯��
						Animation animation =AnimationUtils.loadAnimation
								(ColorCardActivity.this, R.anim.myanim);
						//�����������
						identifyTabView.startAnimation(animation);	
					}
				});
		
		
	}//onCreate���˽���
	
	private void initIdentifyTableView() {
		// TODO �Զ����ɵķ������
		//��������弰��ʼ��
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
//	   	 �޸�uri�˴���ȡ�洢��������ͼƬ�ļ�temp��������ע�ⴢ��Ȩ�޶�ȡ����
	    capfile = new File(Environment.getExternalStorageDirectory(),"temp.jpg");

	    //��ʼ��ͼƬĬ��_���ر���ͼƬ
	    Bitmap bmp = BitmapFactory.decodeResource(getResources(), R.drawable.bg01);
	    imageview = identifyTabView.findViewById(R.id.viewPicture);
	    imageview.setBackgroundDrawable(new BitmapDrawable(bmp));
		//�Ӵ�����ʼ��ɫ����_��ʽ#XXXXXX_RGB��XΪ��ɫ���ݣ�_��׽����
	    imageview.setOnTouchListener(new View.OnTouchListener() {	
			@Override
			public boolean onTouch(View v, MotionEvent event) {
				// TODO �Զ����ɵķ������
				//��������
				int x=(int)event.getX();
				int y=(int)event.getY();
				int h=imageview.getHeight();
				int w=imageview.getWidth();
				//�жϴ�����Χ
				if(x<0||x>=w||y<0||y>=h){
					textColorInfo.setText("������Χ!");
					return true;
				}
				//��δ��ʼ��ͼ�����Ȼ�ȡͼ������
					if(pickbmp ==null){
						Bitmap bgbmp=((BitmapDrawable)imageview.getBackground()).getBitmap();
						pickbmp = Bitmap.createScaledBitmap(bgbmp,w,h,false);
					}
					//��ʾ��ǰ������ɫ
					int pixel=pickbmp.getPixel(x, y);
						
				    viewPickedColor.setBackgroundColor(pixel);
				    //��ʾ16����RGBֵ����ʽ#XXXXXX_RGB��XΪ��ɫ���ݣ�
				    String rgb ="#"+Integer.toHexString(pixel).substring(2).toUpperCase();
					
				    textColorInfo.setText("��ɫ���ݣ�"+rgb+"(��ʽ#_R_G_B)" );
				    editTextRGB.setText(rgb);
				    Bluetooth_transmission();
					int r=Color.red(pixel);
				    int g=Color.green(pixel);
				    int b=Color.blue(pixel);
				    float[] hsv=new float[3];
				    Color.RGBToHSV(r, g, b, hsv);
				    //
				    //TODO��ȡ�������ɫ������ʾ�ڽ�����
				    //
				    // ����ǰ������ɫ���ɫ���ġ����롱��ɫ��ֵ��ŵ�Map��n
					Map<Double, ColorSample> mapHSV = new HashMap<Double, ColorSample>();
					//��������ɫ�����ݱ��������ɫ
					for (ColorSample sample : sampleList) {
						double dHSV = ColorSample.distHSV(hsv[0], hsv[1], hsv[2],
								sample.h, sample.s, sample.v);
						// ����һ�������΢��ֵ�������ظ�����Ϊ���ܴ��ڶ��ɫ����ָ����ɫ�ľ������
						dHSV = dHSV + Math.random() / 1000000.0;
						mapHSV.put(dHSV, sample);
					}
					// ����ɫ��ɫ��ֵ���д�С����
					List<Double> distHSVList = new ArrayList<Double>(mapHSV
							.keySet());
					Collections.sort(distHSVList);
					// �õ���ӽ���������ɫ
					ColorSample hitted01 = mapHSV.get(distHSVList.get(0));
					ColorSample hitted02 = mapHSV.get(distHSVList.get(1));
					ColorSample hitted03 = mapHSV.get(distHSVList.get(2));
					//������λС����ʽ��ӡ
					String diff01 = String.format("%.4f", distHSVList.get(0));
					String diff02 = String.format("%.4f", distHSVList.get(1));
					String diff03 = String.format("%.4f", distHSVList.get(2));
					textColorDiff.setText("����ֵ��" + diff01 + ", " + diff02 + ", "
							+ diff03);
					// ��ʾ�ҵ���������ɫ
					sample01.setBackgroundColor(hitted01.val);
					sample02.setBackgroundColor(hitted02.val);
					sample03.setBackgroundColor(hitted03.val);
					sample01.setText(hitted01.rgb + "\n" + hitted01.name);
					sample02.setText(hitted02.rgb + "\n" + hitted02.name);
					sample03.setText(hitted03.rgb + "\n" + hitted03.name);
					// ����ڴ�
					distHSVList.clear();
					mapHSV.clear();
				    
				return true;
			}
		});
	    
	  //��������ģʽ�� �����¼�
	    btnSetmode.setOnClickListener(new OnClickListener(){
			@Override
			public void onClick(View v) {
				// TODO �Զ����ɵķ������
				//�����޸�ģʽ
				if(bluetoothSocket!=null) {
				String send_str = " SET_MODE\r\n";
				byte[] bf = new byte[32];
				bf = send_str.getBytes();//ת��Ϊ�ֽ�
				 try {
              	   outputStream = bluetoothSocket.getOutputStream();
              	   outputStream.write(bf);
              	   outputStream.write('\0');    // Send an ending sign
                 } catch (IOException e) {
                     e.printStackTrace();
                     Toast.makeText(ColorCardActivity.this, "��������ʧ��!", Toast.LENGTH_SHORT).show();
                     return;
                 }
                 //ǿ���������ջ�����
                 try {
                     if (outputStream != null) {
                  	   outputStream.flush();
                     }
                 } catch (IOException e) {
                     e.printStackTrace();
                     //��������ʧ��
                     Toast.makeText(ColorCardActivity.this, "��������ʧ��", Toast.LENGTH_SHORT).show();
                 }
             }else{
          	   Toast.makeText(ColorCardActivity.this, "���Ƚ�����������", Toast.LENGTH_SHORT).show();
          	   return;
             }
			}
	    	
	    });
	    //
	    //
	    //TODO����ϵͳȨ�ޡ����������������
	    //
	    //�����������ա� �����¼�
	    btnCamera.setOnClickListener(new OnClickListener(){
			@Override
			public void onClick(View v) {
				// TODO �Զ����ɵķ������
				//����ϵͳ���
				Intent intent  = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
//				//ָ���������ݣ����������Ƭ
//				intent.putExtra("return-data", true);
//				�޸�uri��������
				intent.putExtra(MediaStore.EXTRA_OUTPUT,Uri.fromFile(capfile));
				startActivityForResult(intent,PHOTO_CAPTURE);

			}
	    	
	    });
	    
	  //�����������á� �����¼�
	    btnRest.setOnClickListener(new OnClickListener(){
			@Override
			public void onClick(View v) {
				// TODO �Զ����ɵķ������
				//��ʼ��ͼƬĬ��_���ر���ͼƬ
			    Bitmap bmp = BitmapFactory.decodeResource(getResources(), R.drawable.bg01);
			    imageview = identifyTabView.findViewById(R.id.viewPicture);
			    imageview.setBackgroundDrawable(new BitmapDrawable(bmp));
			    pickbmp = null;

			}
	    	
	    });
	    //
	    //TODO����ϵͳȨ�ޡ������������ѡȡͼƬ
	    //
	    //����������ᡱ �����¼�
	    photo_album.setOnClickListener(new OnClickListener(){
			@Override
			public void onClick(View v) {
				// TODO �Զ����ɵķ������
//				�˷�������4.4���°汾��������˵����������c��̳***������Դ�����ڴ˱�ʾ��л!!!
				Intent intent = new Intent(Intent.ACTION_PICK,android.provider.MediaStore.Images.Media.EXTERNAL_CONTENT_URI);
				intent.setDataAndType(MediaStore.Images.Media.EXTERNAL_CONTENT_URI,"image/*");
				startActivityForResult(intent,PHOTO_REQUEST_GALLERY);
				//����ϵͳ���
//                Intent intent = new Intent(Intent.ACTION_GET_CONTENT); //4.4�Ƽ��ô˷�ʽ��4.4���µ�API��Ҫ�ټ���
//                intent.addCategory(Intent.CATEGORY_OPENABLE);
//                intent.setType("image/*");
			}	    	
	    });
	    
	    
	  //�����������á� �����¼�
	    btnSetdata.setOnClickListener(new OnClickListener(){
			@Override
			public void onClick(View v) {
				// TODO �Զ����ɵķ������
				//����RGB����
				Bluetooth_transmission();
				
			}
	    	
	    });
	    //
	    
	  //��Ӧ���ӽ���
	    Link_Set.setOnClickListener(new OnClickListener(){
	  					@Override
	  					public void onClick(View v) {
	  						// TODO �Զ����ɵķ������
	  						//�����������ݷ���
	  						Set<BluetoothDevice> devices = bluetoothAdapter.getBondedDevices();
	  				        bluetoothDevice = bluetoothAdapter.getRemoteDevice(blueAddress);
	  				        if(bluetoothSocket==null){
	  				        	try{
	  						           bluetoothSocket = bluetoothDevice.createRfcommSocketToServiceRecord(MY_UUID_SECURE);
	  						           bluetoothSocket.connect();
	  						         Link_Set.setText("OK");
	  						       }catch (IOException e){
	  						           e.printStackTrace();
	  						         Link_Set.setText("����");
	  						           Toast.makeText(ColorCardActivity.this, "�������ӽ���ʧ��!", Toast.LENGTH_SHORT).show();
	  						       }
	  				        }
	  				        else {
	  				        	//Toast.makeText(ig_m.this, "�������������÷���", Toast.LENGTH_SHORT).show();
	  				        }	        	

	  					}	
	  				});
	    
	    
	}
	//ϵͳ ���ǡ�/ʵ�ַ���
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		// TODO �Զ����ɵķ������
		//�����;ȡ������Ƭ�Ĵ�����ֱ���˳�
		if(resultCode==Activity.RESULT_CANCELED){
			return;
		}
		//�������մ����أ�������ϵͳ����
		else if(requestCode==PHOTO_CAPTURE){	
//				�޸ķ���uri����
				Intent intent = new Intent("com.android.camera.action.CROP");
//				�������ݼ�����_������ָuri���ص�ͼƬ
				intent.setDataAndType(Uri.fromFile(capfile), "image/*");
				//��Ӧimageview����
				intent.putExtra("aspectX",imageview.getWidth());
				intent.putExtra("aspectY",imageview.getHeight());
				//������ú�ͼƬ��С_��������Ӧimageview
				intent.putExtra("outputX",imageview.getWidth());
				intent.putExtra("outputY",imageview.getHeight());
				//2.ʹ��Uri��Դ����ͼƬ����
				intent.putExtra("output", Uri.fromFile(capfile));
				intent.putExtra("outputFormat","JPEG");
				// ����һ�����з���ֵ��Activity��������ΪPHOTO_CROP
				startActivityForResult(intent,PHOTO_CROP);

		}
		
//		���÷���
		else if (requestCode == PHOTO_CROP){
//			�޸�uri����
			if(capfile.exists()){
				Bitmap photo = BitmapFactory.decodeFile(capfile.getAbsolutePath());
				imageview.setBackgroundDrawable(new BitmapDrawable(photo));
			    pickbmp = null;
			}
		}
//		��᷵��
		else if (requestCode == PHOTO_REQUEST_GALLERY){
			 // ����᷵�ص�����
			 if (data != null&&resultCode == Activity.RESULT_OK) {
//				 String[] projection = { MediaStore.Images.Media.DATA };
//				    Cursor cursor = context.getContentResolver().query(contentUri, projection, null, null, null);
//				    int column_index = cursor.getColumnIndexOrThrow(MediaStore.Images.Media.DATA);
//				    cursor.moveToFirst();
//				    filePath = cursor.getString(column_index);
				 //�õ���������
				 Uri uri = data.getData();
//				 String[] pojo = {MediaStore.Images.Media.DATA};
//				 Cursor cursor = managedQuery(uri, pojo, null, null, null);
//				 if (cursor != null) {
//					 ContentResolver cr = this.getContentResolver();
//                     int colunm_index = cursor.getColumnIndexOrThrow(MediaStore.Images.Media.DATA);
//                     cursor.moveToFirst();
//                     String path = cursor.getString(colunm_index);
//			 }
//                     //���ϴ����ȡͼƬ·��
                     
     				Intent intent = new Intent("com.android.camera.action.CROP");
//    				�������ݼ�����_������ָuri���ص�ͼƬ
    				intent.setDataAndType(uri, "image/*");
    				//��Ӧimageview����
    				intent.putExtra("aspectX",imageview.getWidth());
    				intent.putExtra("aspectY",imageview.getHeight());
    				//������ú�ͼƬ��С_��������Ӧimageview
    				intent.putExtra("outputX",imageview.getWidth());
    				intent.putExtra("outputY",imageview.getHeight());
    				//2.ʹ��Uri���ͼƬ����
    				intent.putExtra("output", Uri.fromFile(capfile));
    				intent.putExtra("outputFormat","JPEG");
    				// ����һ�����з���ֵ��Activity��������ΪPHOTO_CROP
    				startActivityForResult(intent,PHOTO_CROP);
                    
//				 Toast.makeText(ColorCardActivity.this,"�汾�����׳����˶γ����޸�_andriod4.4���¼���",
//							Toast.LENGTH_SHORT).show();
//				�����ڴ������OOM���������_nice
//    				��ӹ���_���ѡȡ��Ƭ
				 
			 }
				
		}
		
		super.onActivityResult(requestCode, resultCode, data);
	}
	
	/**�ع�����
	 * ����RGB����
	 * ͨ�������������ݵ���λ���豸
	 */
	private void Bluetooth_transmission(){
		//����
		final EditText editTextRGB = (EditText)identifyTabView.findViewById(R.id.editTextRGB);
		if(bluetoothSocket!=null) {
		String send_str = " ";
		send_str = send_str+editTextRGB.getText()+" \r\n";
		byte[] bf = new byte[64];
		bf = send_str.getBytes();//ת��Ϊ�ֽ�
		 try {
      	   outputStream = bluetoothSocket.getOutputStream();
      	   outputStream.write(bf);
      	   outputStream.write('\0');    // Send an ending sign
         } catch (IOException e) {
             e.printStackTrace();
             Toast.makeText(ColorCardActivity.this, "��������ʧ��!", Toast.LENGTH_SHORT).show();
             return;
         }
         //ǿ���������ջ�����
         try {
             if (outputStream != null) {
          	   outputStream.flush();
             }
         } catch (IOException e) {
             e.printStackTrace();
             //��������ʧ��
             Toast.makeText(ColorCardActivity.this, "��������ʧ��", Toast.LENGTH_SHORT).show();
         }
     }else{
  	   Toast.makeText(ColorCardActivity.this, "���Ƚ�����������", Toast.LENGTH_SHORT).show();
  	   return;
     }
	}

	/**�ع�����
	 * ����Tab������ʱ������״̬
	 * ��ʾ������ʱ��ѡ�
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
	
	/**�ع�����
	 * ��ȡɫ������
	 */
	private void loadColorCards(){
		//��ȡ���ݰ���\n�����зָ�
		String sampleColors = getResources().getString(
				R.string.sample_color_list);
		String[] ss = sampleColors.split("\n");
		//���Ͳ�ת��ɫ������  ��ʽ��ʮ������RGB������ɫ����[�������]
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
				//������Чɫ��
				if(rgb!=null&&name!=null&&category!=null){
					sampleList.add(new ColorSample(rgb,name,category));
				}
			}
		}	
	}
	/**�ع�����
	 * ��ʼ��ɫ������
	 * ����ɫ��������Դ��sampleList
	 */
	private void initSampleTabView(){
		//��ȡ�����ܶ�
		final float scale = getResources().getDisplayMetrics().density;
		//���Դɫ��
		TableLayout sampleTable = (TableLayout)sampleTabView.findViewById(R.id.sampleTable);
		sampleTable.removeAllViews();
		//��̬���ɫ��
		for(final ColorSample samp:sampleList){
			TableRow row = new TableRow(this);
			row.setPadding(0, (int)(20*scale+0.5f), 0, 0);
			View co100 = new View(this);
			co100.setBackgroundColor(samp.val);
			co100.setMinimumHeight((int)(80*scale+0.5f));
			TextView co101 = new TextView(this);
			co101.setText(samp.name);
			//�����С��ɫ����
			co101.setTextSize(20);
			co101.setTextColor(Color.DKGRAY);
			co101.setGravity(Gravity.CENTER);
			co101.setHeight((int)(80*scale+0.5f));
			row.addView(co100);
			row.addView(co101);
			sampleTable.addView(row);  
		}
	}
	
	/**���ܺ���
	 * ��������
	 */
	private void initSearchTabView(){
		//��ȡ��������ݣ���ʾ��   ע������ʹ�õ���������붨����Դ_�������ǿ��ֹͣ����
		Button btnSearch =(Button)searchTabView.findViewById(R.id.btnSearch);
		final TableLayout resultTable = 
				(TableLayout)searchTabView.findViewById(R.id.resultTable);
		final EditText editColorName = 
				(EditText)searchTabView.findViewById(R.id.editColorName);
		//����"����"���������¼�������������
		btnSearch.setOnClickListener(new OnClickListener(){
			@Override
			public void onClick(View v) {
				//����ɫ�����ݱ���ա�����ʾ��
				List<ColorSample> sampleList_result=new ArrayList<ColorSample>();	
				
				//�����������ѷ�����������ʾ����_�쳣����ʾ����
				try{
					//��ȡ�ַ������"ɫ"��ȥ����ɫ���֣���ʵ��ģ����������������������������
					String search_con=editColorName.getText().toString();
	            	if (search_con.contains("ɫ")){
	            		//�ַ���ȡ�Ӵ�ȥ����ɫ����
	            		search_con = search_con.substring(0, search_con.length()-1);
	            	}
	            	//��������ɫ�����ݱ�������֡�ƥ��ɹ�������ӵ�sampleList_result�嵥��
	            	for(final ColorSample samp2: sampleList){
	            		if(samp2.name.contains(search_con)==true){
	            			sampleList_result.add(samp2);
	            		}
	            	}
	            	//��ȡ�����ܶ�
	                final float scale=getResources().getDisplayMetrics().density;
	        	    resultTable.removeAllViews();
	            	//������̬ɫ�������ڼ���ɫ�������������-����ο�sample�����ʼ�������������Ͻ���
	            	for(final ColorSample samp3:sampleList_result){
	        			TableRow row = new TableRow(ColorCardActivity.this);
	        			row.setPadding(0, (int)(20*scale+0.5f), 0, 0);
	        			View co100 = new View(ColorCardActivity.this);
	        			co100.setBackgroundColor(samp3.val);
	        			co100.setMinimumHeight((int)(80*scale+0.5f));
	        			TextView co101 = new TextView(ColorCardActivity.this);
	        			co101.setText(samp3.name);
	        			//�����С��ɫ����
	        			co101.setTextSize(20);
	        			co101.setTextColor(Color.DKGRAY);
	        			
	        			co101.setGravity(Gravity.CENTER);
	        			co101.setHeight((int)(80*scale+0.5f));
	        			row.addView(co100);
	        			row.addView(co101);
	        			resultTable.addView(row);  
	        			
	        		}
				}
				//�쳣����
				catch (Exception e){
					Toast.makeText
					(ColorCardActivity.this,"����δ֪����,����!",Toast.LENGTH_SHORT).show();
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
			//��ӹ����˳�
		       try {
		              // ����ʱͼƬ�ļ�ɾ��
			          capfile.delete();
		       }
			       catch (Exception e) {
			    	   //��ʧ�����ӡ������Ϣ�ڳ����г����λ�ü�ԭ��
			             e.printStackTrace();
				         }	
			finish();
			return true;
		}
		return super.onOptionsItemSelected(item);
	}

	@Override
//	�Զ����ɷ��������˳�ȷ��
	public void onBackPressed() {
		// TODO �Զ����ɵķ������
        new AlertDialog.Builder(this).setTitle("ͼƬ�ļ�����ɾ����ȷ���˳���") 
        .setIcon(android.R.drawable.ic_dialog_info) 
        .setPositiveButton("ȷ��", new DialogInterface.OnClickListener() { 
 
            @Override 
            public void onClick(DialogInterface dialog, int which) { 
            // �����ȷ�ϡ���Ĳ��� 
 		       try {
		              // ����ʱͼƬ�ļ�ɾ��
			          capfile.delete();
		       }
			       catch (Exception e) {
			    	   //��ʧ�����ӡ������Ϣ�ڳ����г����λ�ü�ԭ��
			             e.printStackTrace();
				         }	
             finish(); 
     
            } 
        }) 
        .setNegativeButton("�ٿ���", new DialogInterface.OnClickListener() { 
     
            @Override 
            public void onClick(DialogInterface dialog, int which) { 
            // ��������ء���Ĳ���,���ﲻ����û���κβ��� 
            } 
        }).show(); 
        
//		super.onBackPressed();
	}
}
