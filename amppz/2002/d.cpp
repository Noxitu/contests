#include "../biblioteczka.hpp"

int main() { return main_many(); }

void test()
{
    int n;
    cin >> n;
    vector<double> values(n*n);
    cin >> values;
    
    int col_zero = 0;
    double col_sum = 0.0;
    
    auto close_full_col = [&](int col) {
        //if( col >= col_zero )
        if( col_sum > 0.0 )
            cout << col_zero << " 0 " << col << ' ' << n-1 << '\n';
    };
    
    for(int col = 0; col < n; ++col)
    {
        int row_zero = 0;
        double sum = 0.0;
        double row_sum = 0.0;
        bool row_cleared = false;
        
        auto close_row = [&](int row) {
            //if(row >= row_zero)
            if(row_sum > 0.0)
                cout << col << ' ' << row_zero << ' ' << col << ' ' << row << '\n';
        };
        
        for(int row = 0; row < n; ++row)
        {
            double value = values[col + n*row];
            if(row_sum+value > 2.0)
            {
                row_cleared = true;
                
                close_full_col(col-1);
                col_zero = col + 1;
                col_sum = 0.0;
                
                close_row(row-1);
                row_zero = row;
                row_sum = 0.0;
            }
            
            row_sum += value;
            sum += value;
        }
        
        if( row_cleared )
        {
            close_row(n-1);
        }
        else 
        {
            if( col_sum + sum > 2.0 )
            {
                close_full_col(col-1);
                col_zero = col;
                col_sum = 0.0;
            }
            col_sum += sum;
        }
    }
    
    close_full_col(n-1);
    cout << "-1\n";
}