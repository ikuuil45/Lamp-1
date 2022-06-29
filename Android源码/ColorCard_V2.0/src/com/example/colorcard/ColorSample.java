package com.example.colorcard;

import android.graphics.Color;

public class ColorSample {
	public final String rgb;
	public final String name;
	public final String category;
	public final int val;
	//定义三原色
    public final int r;
    public final int g;
    public final int b;
    
    public final float h;
    public final float s;
    public final float v;
	
	public ColorSample(String rgb,String name,String category){
		this.rgb = rgb;
		this.name = name;
		this.category = category;
		//转换三原色值
		val = Color.parseColor(rgb); 
		r=Color.red(val);
		g=Color.green(val);
		b=Color.blue(val);
		//再次转换
		float[] hsv=new float[3];
		Color.RGBToHSV(r, g, b, hsv);
		h=hsv[0];
		s=hsv[1];
		v=hsv[2];
	}
	/*公共类函数
	 * 计算HSV色差
	 * 
	 * */
	public static double distHSV(double h1,double s1,double v1,double h2,double s2,double v2){
   	 return Math.sqrt((h1-h2)*(h1-h2)+(s1-s2)*(s1-s2)+(v1-v2)*(v1-v2));
    }
	/*公共类函数
	 * 计算RGB色差
	 * 
	 * */
	public static double distRGB(int r1,int g1,int b1,int r2,int g2,int b2){
   	 return Math.sqrt((r1-r2)*(r1-r2)+(g1-g2)*(g1-g2)+(b1-b2)*(b1-b2));
    }
}
