import { Component, OnInit } from "@angular/core";
import { HttpClient } from "@angular/common/http";

@Component({
  selector: "app-root",
  templateUrl: "./app.component.html",
  styleUrls: ["./app.component.css"],
})
export class AppComponent implements OnInit {
  public tableData: any;
  public proximityAmount: number;
  public waterTapAmount: number;
  constructor(private http: HttpClient) {}

  ngOnInit() {
    this.getData();
    setInterval(() => {
      this.getData();
    }, 2500);
  }

  getData() {
    this.http
      .get("https://timetowashyourhands.herokuapp.com/getData")
      .subscribe((data) => {
        this.tableData = data;
        this.proximityAmount = this.getProximityAmount(data);
        this.waterTapAmount = this.getWaterTapAmount(data);
        console.log(this.tableData);
      });
  }
  getProximityAmount(table): number {
    let count = 0;
    for (let i = 0; i < table.length; i++) {
      if (i == 0) {
        if (table[i][2] == 1) {
          count++;
        }
      } else {
        if (table[i - 1][2] == 0 && table[i][2] == 1) {
          count++;
        }
      }
    }
    console.log(count);
    return count;
  }
  getWaterTapAmount(table): number {
    let filteredTable = table.filter((element) => {
      return element[1] == 1 && element[3] === "occupied";
    });
    return filteredTable.length;
  }

  turnOnSprayer() {
    this.http
      .get("https://timetowashyourhands.herokuapp.com/spray")
      .subscribe((data) => {
        window.alert("Sprayer turned on");
        console.log(data);
      });
  }
}
