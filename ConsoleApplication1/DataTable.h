#pragma once
#include <vector>
#include <algorithm>
#include <numeric>


namespace w7 {
	//Templated class that can find mean, median, regression and sigma
	template<typename T>
	class DataTable {
		std::vector<T> Xdata;
		std::vector<T> Ydata;
		int field_width;
		int no_decimals;
	public:
		DataTable(std::ifstream& fs, int ui_field_width, int ui_no_decimals)
		{
			field_width = ui_field_width;
			no_decimals = ui_no_decimals;
			T value;
			int i = 0;

			while (!fs.eof())
			{
				fs >> value;

				if (i == 0)
				{
					Xdata.push_back(value);
					i++;
				}
				else {
					Ydata.push_back(value);
					
					i--;
				}
			}
		}
		T mean() const { //mean of Ydata only
			T myMean = std::accumulate(Ydata.begin(), Ydata.end(), (T)0) / Ydata.size(); //0 is initilaizer
			return myMean;
		}
		T median() const {
			//init a container and 
			std::vector<T>temp(Ydata.size());
			std::copy(Ydata.begin(), Ydata.end(), temp.begin());
			int MedianIndex;

			std::sort(temp.begin(), temp.end());
			if (temp.size() % 2 ==0) //if its an even array
			{
				MedianIndex = temp.size() / 2;
				return temp[MedianIndex];
			}
			else
			{
				MedianIndex = (temp.size() - 1) / 2;
				return temp[MedianIndex];
			}
		}

		T sigma() const {
			T result = std::sqrt(std::accumulate(Ydata.begin(), Ydata.end(), (T)0, [=](T currentAccumilation, T currentValue) 
			{
				return (currentAccumilation + (currentValue - mean()) * (currentValue - mean()));
			})/(Ydata.size()-1));
			return result;
		}

		void regression(T& slope, T& y_intercept) const
		{
			T sum_x = std::accumulate(Xdata.begin(), Xdata.end(), (T)0);
			T sum_Y = std::accumulate(Ydata.begin(), Ydata.end(), (T)0);

			T XmltipliedBy_X = std::inner_product(Xdata.begin(), Xdata.end(), Xdata.begin(), (T)0);
			T YmltipliedBy_X = std::inner_product(Ydata.begin(), Ydata.end(), Xdata.begin(), (T)0);

			slope = (Xdata.size() *YmltipliedBy_X - sum_x * sum_Y) / (Xdata.size() * XmltipliedBy_X - sum_x * sum_x);
			y_intercept = (sum_Y - slope * sum_x) / Xdata.size();

		}

		void display(std::ostream& os) const {
			os.setf(std::ios::fixed);

			os.width(field_width);
			os << "X";
			os.width(field_width);
			os << "Y" << std::endl;
			for(int i=0; i< Xdata.size(); i++)
			{
				os.precision(no_decimals);
				os.width(field_width);
				os << Xdata[i];
				os.width(field_width); 
				os << Ydata[i] << std::endl;
			}
		}
	};

	template <typename T>
	std::ostream& operator<<(std::ostream& os, const DataTable<T>& dt)
	{
		dt.display(os);
		return os;
	}

}