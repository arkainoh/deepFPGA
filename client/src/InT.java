

import java.awt.Button;
import java.awt.Color;
import java.awt.FlowLayout;
import java.awt.Frame;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.Panel;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseMotionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

public class InT extends Frame implements MouseMotionListener {
	private static final long serialVersionUID = -2592446912051617238L;
	private static final int[][] POINT_COLOR = {{0x34, 0x79, 0x34},{0x79, 0xfe, 0x79},{0x34, 0x79, 0x34}};
	private static final int DATA_SIZE = 28;
	private static final int SCALE = 10;
	
	private int[][] data;
	private int x;
	private int y;
	private int penSize;
	private int eraserSize;
	private Image img;
	private Graphics gImg;
	
	public static void main(String[] args) {
		new InT("InT");
	}
	
	public InT(String title) {
		super(title);
		this.x = 0;
		this.y = 0;
		this.penSize = SCALE * 3;
		this.eraserSize = SCALE * 5;
		this.data = new int[DATA_SIZE][DATA_SIZE];
		
		setLayout(null);
		addMouseMotionListener(this);
		addWindowListener(new WindowAdapter() {

			@Override
			public void windowClosing(WindowEvent e) {
				// TODO Auto-generated method stub
				System.exit(0);
			}
		});
		
		setBounds(100, 100, DATA_SIZE * SCALE, DATA_SIZE * SCALE + 35);
		setResizable(false);
		setVisible(true);
		Panel p = new Panel();
		p.setLayout(new FlowLayout());
		p.setBackground(Color.GRAY);
		p.setSize(DATA_SIZE * SCALE, 35);
		p.setLocation(0, DATA_SIZE * SCALE);
		Button clearButton = new Button("clear");
		clearButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e){
				clear();
			}
		});
		Button sendButton = new Button("send");
		sendButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e){
				printData(); // should be changed into sendData()
			}
		});
		p.add(clearButton);
		p.add(sendButton);
		add(p);
		
		img = createImage(DATA_SIZE * SCALE, DATA_SIZE * SCALE);
		gImg = img.getGraphics();
		repaint();
	}

	@Override
	public void mouseDragged(MouseEvent e) {
		// TODO Auto-generated method stub
		this.x = e.getX();
		this.y = e.getY();
		if(e.getModifiersEx() == MouseEvent.BUTTON1_DOWN_MASK) {
			gImg.setColor(Color.BLACK);
			gImg.fillOval(this.x, this.y, this.penSize, this.penSize);
			drawPoint(this.x, this.y);
		} else if(e.getModifiersEx() == MouseEvent.BUTTON3_DOWN_MASK) {
			gImg.setColor(Color.WHITE);
			gImg.fillOval(this.x, this.y, this.eraserSize, this.eraserSize);
		}
		repaint();
	}

	@Override
	public void mouseMoved(MouseEvent e) {
		// TODO Auto-generated method stub

	}

	public void paint(Graphics g) {
		if(img != null)
			g.drawImage(img, 0, 0, this);
	}
	
	private void clear() {
		for(int i = 0; i < DATA_SIZE; i++)
			for(int j = 0; j < DATA_SIZE; j++)
				this.data[i][j] = 0;
		gImg.clearRect(0, 0, DATA_SIZE * SCALE, DATA_SIZE * SCALE);
		repaint();
	}
	
	private void drawPoint(int x, int y) {
		x = x/10;
		y = y/10;
		if(x <= 0 || x >= DATA_SIZE - 1 || y <= 0 || y >= DATA_SIZE - 1)
			return;
		x--; y--;
		for(int i = 0; i < this.penSize/SCALE; i++) {
			for(int j = 0; j < this.penSize/SCALE; j++) {
				if(data[y+i][x+j] < POINT_COLOR[i][j])
					data[y+i][x+j] = POINT_COLOR[i][j];
			}
		}
	}
	
	private void printData() {
		for(int i = 0; i < 28; i++) {
			for(int j = 0; j < 28; j++) {
				System.out.print(String.format("%02x ", data[i][j]));
			}
			System.out.println();
		}
	}
}
